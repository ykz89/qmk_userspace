// Stand-in for QMK's split transactions. One simulated half, so RPCs go nowhere.
#pragma once
#include <stdbool.h>
#include <stdint.h>
enum { RPC_ID_BKLM_POINTER_SYNC };
typedef void (*slave_callback_t)(uint8_t, const void *, uint8_t, void *);
void transaction_register_rpc(int8_t id, slave_callback_t cb);
bool transaction_rpc_send(int8_t id, uint8_t len, const void *data);
