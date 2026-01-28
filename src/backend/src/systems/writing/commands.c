#include <stdlib.h>
#include "systems/writing/internal.h"
#include "systems/writing/commands.h"

#define DATA_ALLOC 3

// +===----- Buffer -----===+ //

bool	cmd_buffer_create(t_Manager *manager, t_Command *cmd)
{
	t_WritingCtx		*_ctx;
	t_Buffer			*_buffer;
	t_Buffer			*_tmp;
	t_CmdCreateBuffer	*_payload;

	if (NULL == manager || NULL == cmd)
		return (false);
	_ctx = manager->writing_ctx;
	if (NULL == _ctx)
		return (false);
	if (cmd->payload_size && cmd->payload_size != sizeof(t_CmdCreateBuffer))
		return (false);
	_payload = cmd->payload;
	if (NULL == _ctx->buffers)
	{
		_ctx->buffers = malloc(DATA_ALLOC * sizeof(t_Buffer *));
		if (NULL == _ctx->buffers)
			return (false);
		_ctx->capacity = DATA_ALLOC;
	}
	if (_ctx->count >= _ctx->capacity)
	{
		_tmp = realloc(
			_ctx->buffers,
			(_ctx->capacity + DATA_ALLOC) * sizeof(t_Buffer *)
		);
		if (NULL == _tmp)
			return (false);
		_ctx->buffers = _tmp;
		_ctx->capacity += DATA_ALLOC;
	}
	_buffer = buffer_create();
	if (NULL == _buffer)
		return (false);
	_ctx->buffers[_ctx->count] = _buffer;
	_payload->out_buffer_id = _ctx->count;
	_ctx->count++;
	return (true);
}

bool	cmd_buffer_destroy(t_Manager *manager, t_Command *cmd);

// +===----- Lines -----===+ //

bool	cmd_line_create(t_Manager *manager, t_Command *cmd);

bool	cmd_buffer_line_destroy(t_Manager *manager, t_Command *cmd);

bool	cmd_buffer_line_insert(t_Manager *manager, t_Command *cmd);

bool	cmd_buffer_line_split(t_Manager *manager, t_Command *cmd);

bool	cmd_buffer_line_join(t_Manager *manager, t_Command *cmd);

bool	cmd_buffer_get_line(t_Manager *manager, t_Command *cmd);

// +===----- Data -----===+ //

bool	cmd_line_add_data(t_Manager *manager, t_Command *cmd);

bool	cmd_line_delete_data(t_Manager *manager, t_Command *cmd);