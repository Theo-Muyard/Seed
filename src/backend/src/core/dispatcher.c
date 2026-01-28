#include <stdlib.h>
#include "dispatcher.h"

/**
 * @brief Search for the function associated with the given ID.
 * @param dispatcher The dispatcher that will contains commands.
 * @param id The id of the command.
*/
static t_Fn	search_function(t_Dispatcher *dispatcher, t_CommandId id)
{
	size_t	_count;
	size_t	_i;

	if (NULL == dispatcher)
		return (NULL);
	_i = 0;
	_count = dispatcher->count;
	while (_i < _count)
	{
		if (dispatcher->commands[_i].id == id)
			return (dispatcher->commands[_i].fn);
		_i++;
	}
	return (NULL);
}

t_Dispatcher	*dispatcher_create(size_t capacity)
{
	t_Dispatcher	*dispatcher;

	dispatcher = malloc(sizeof(t_Dispatcher));
	if (NULL == dispatcher)
		return (NULL);
	dispatcher->count = 0;
	dispatcher->capacity = capacity;
	dispatcher->commands = malloc(capacity * sizeof(t_CommandEntry));
	if (NULL == dispatcher->commands)
		return (free(dispatcher), NULL);
	return (dispatcher);
}

bool	dispatcher_destroy(t_Dispatcher *dispatcher)
{
	if (NULL == dispatcher)
		return (false);
	free(dispatcher->commands);
	dispatcher->commands = NULL;
	dispatcher->count = 0;
	dispatcher->capacity = 0;
	free(dispatcher);
	return (true);
}

bool	dispatcher_register(t_Dispatcher *dispatcher, t_CommandId id, t_Fn fn)
{
	t_CommandEntry	_entry;
	ssize_t			_count;

	if (NULL == dispatcher || NULL == fn)
		return (false);
	if (NULL == dispatcher->commands)
		return (false);
	_count = dispatcher->count;
	if (_count >= dispatcher->capacity)
		return (false);
	_entry.id = id;
	_entry.fn = fn;
	dispatcher->commands[_count] = _entry;
	dispatcher->count++;
	return (true);
}

bool	dispatcher_exec(t_Dispatcher *dispatcher, t_CommandCtx *ctx, const t_Command *cmd)
{
	t_Fn		_fn;
	bool		state;

	if (NULL == dispatcher || NULL == ctx || NULL == cmd)
		return (false);
	_fn = search_function(dispatcher, cmd->id);
	if (NULL == _fn)
		return (false);
	state = _fn(ctx, cmd);
	return (state);
}
