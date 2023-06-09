/* PipeWire */
/* SPDX-FileCopyrightText: Copyright © 2018 Wim Taymans */
/* SPDX-License-Identifier: MIT */

#ifndef PIPEWIRE_SPA_NODE_H
#define PIPEWIRE_SPA_NODE_H

#include <spa/node/node.h>

#include <pipewire/impl.h>

#ifdef __cplusplus
extern "C" {
#endif

enum pw_spa_node_flags {
	PW_SPA_NODE_FLAG_ACTIVATE	= (1 << 0),
	PW_SPA_NODE_FLAG_NO_REGISTER	= (1 << 1),
	PW_SPA_NODE_FLAG_ASYNC		= (1 << 2),
};

struct pw_impl_node *
pw_spa_node_new(struct pw_context *context,
		enum pw_spa_node_flags flags,
		struct spa_node *node,
		struct spa_handle *handle,
		struct pw_properties *properties,
		size_t user_data_size);

struct pw_impl_node *
pw_spa_node_load(struct pw_context *context,
		 const char *factory_name,
		 enum pw_spa_node_flags flags,
		 struct pw_properties *properties,
		 size_t user_data_size);

void *pw_spa_node_get_user_data(struct pw_impl_node *node);

#ifdef __cplusplus
}
#endif

#endif /* PIPEWIRE_SPA_NODE_H */
