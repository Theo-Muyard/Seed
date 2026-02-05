#include <stdlib.h>
#include <string.h>
#include "core/manager.h"
#include "core/dispatcher.h"
#include "tools/memory.h"
#include "systems/filesystem/_internal.h"
#include "systems/filesystem/_os.h"
#include "systems/filesystem/system.h"

// +===----- Commands Definition -----===+ //

// TODO: Complete
const t_CommandEntry	fs_commands[] = {
};

/**
 * @brief Registers the commands into the dispatcher.
 * @param dispatcher The dispatcher that will contains commands.
 * @param commands The container for all write command entries.
 * @param count The count of commands.
*/
static bool	register_commands(t_Dispatcher *dispatcher, const t_CommandEntry *commands, size_t count)
{
	size_t	_i;

	_i = 0;
	while (_i < count)
	{
		if (false == dispatcher_register(
			dispatcher,
			commands[_i].id,
			commands[_i].size,
			commands[_i].fn)
		)
			return (false);
		_i++;
	}
	return (true);
}

bool	fs_init(t_Manager	*manager, char *root_path)
{
	t_FileSystemCtx		*_ctx;

	if (NULL == manager)
		return (false);
	if (NULL == manager->dispatcher)
		return (false);
	_ctx = malloc(sizeof(t_FileSystemCtx));
	if (NULL == _ctx)
		return (false);
	_ctx->root = malloc(sizeof(t_Directory));
	if (NULL == _ctx->root)
		return (false);
	if (false == register_commands(
		manager->dispatcher,
		fs_commands,
		FS_COMMANDS_COUNT
	))
		return (free(_ctx->root), free(_ctx), false);
	_ctx->root->absolute_path = ft_strdup(root_path);
	if (NULL == _ctx->root->absolute_path)
		return (free(_ctx->root), free(_ctx), false);
	manager->fs_ctx = _ctx;
	return (true);
}

void	fs_clean(t_FileSystemCtx *ctx)
{
	if (NULL == ctx)
		return ;
	directory_destroy(ctx->root);
	free(ctx);
}
