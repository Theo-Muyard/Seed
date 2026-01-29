#include <stdlib.h>
#include "core/manager.h"
#include "core/dispatcher.h"
#include "systems/writing/system.h"
#include "systems/writing/commands.h"

static bool	register_commands(t_Dispatcher *dispatcher, const t_CommandEntry *commands, size_t size)
{
	int	_i;

	_i = 0;
	while (_i < size)
	{
		if (false == dispatcher_register(dispatcher, commands[_i].id, commands[_i].fn))
			return (false);
		_i++;
	}
	return (true);
}

bool	writing_init(t_Manager	*manager)
{
	t_WritingCtx		*_ctx;
	t_Dispatcher		*_dispatcher;

	if (NULL == manager)
		return (false);
	if (NULL == manager->dispatcher)
		return (false);
	_ctx = malloc(sizeof(t_WritingCtx));
	if (NULL == _ctx)
		return (false);
	_ctx->buffers = NULL;
	_ctx->count = 0;
	_ctx->capacity = 0;
	if (false == register_commands(
		manager->dispatcher,
		writing_commands,
		sizeof(writing_commands) / sizeof(*writing_commands)
	))
		return (free(_ctx), false);
	manager->writing_ctx = _ctx;
	return (true);
}

void	writing_clean(t_Manager	*manager)
{
	t_WritingCtx	*_ctx;
	int				_i;

	if (NULL == manager)
		return ;
	_ctx = manager->writing_ctx;
	_i = 0;
	while (_i < _ctx->count)
	{
		buffer_destroy(_ctx->buffers[_i]);
		_i++;
	}
	free(_ctx->buffers);
	_ctx->buffers = NULL;
	_ctx->count = 0;
	_ctx->capacity = 0;
	free(_ctx);
}
