#include <stdlib.h>
#include "systems/writing/system.h"
#include "systems/writing/commands.h"
#include "core/dispatcher.h"

bool	writing_init(t_Manager	*manager)
{
	t_WritingCtx	*_ctx;
	t_Dispatch		*_dispatcher;

	if (NULL == manager)
		return (false);
	if (NULL == manager->dispatch)
		return (false);
	_ctx = malloc(sizeof(t_WritingCtx));
	if (NULL == _ctx)
		return (false);
	_ctx->buffers = NULL;
	_ctx->count = 0;
	_ctx->capacity = 0;
	// TODO: Enregistrer chaque commandes du systeme : array (cmd - fn) et boucler dessus pour enregistrer
	manager->writing_ctx = _ctx;
	return (true);
}

bool	writing_clean(t_Manager	*manager)
{
	t_WritingCtx	*_ctx;
	int				_i;

	if (NULL == manager)
		return (false);
	_ctx = manager->writing_ctx;
	_i = 0;
	while (_i < _ctx->count)
	{
		buffer_destroy(_ctx->buffers[_i]);
		_i++;
	}
	_ctx->buffers = NULL;
	_ctx->count = 0;
	_ctx->capacity = 0;
	free(_ctx);
	return (true);
}
