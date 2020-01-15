#include "sdk_common.h"
//#if NRF_MODULE_ENABLED(BLE_LBS)
#include "lock_service.h"
#include "ble_srv_common.h"

/**@brief Function for handling the Write event.
 *
 * @param[in] p_lock_service      Lock Service structure.
 * @param[in] p_ble_evt           Event received from the BLE stack.
 */
static void on_write(ble_lock_service_t * p_lock_service, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (   (p_evt_write->handle == p_lock_service->ble_input_char_handles.value_handle)
        //&& (p_evt_write->len == 1)
        && (p_lock_service->ble_input_write_handler != NULL))
    {
        p_lock_service->ble_input_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_lock_service, (uint8_t *) p_evt_write->data, p_evt_write->len);
    }
 
    if (   (p_evt_write->handle == p_lock_service->latch_char_handles.value_handle)
        && (p_evt_write->len == 1)
        && (p_lock_service->latch_write_handler != NULL))
    {
        p_lock_service->latch_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_lock_service, p_evt_write->data[0]);
    }
 
}


void ble_lock_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_lock_service_t * p_lock_service = (ble_lock_service_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_lock_service, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_lock_service_init(ble_lock_service_t * p_lock_service, const ble_lock_service_init_t * p_lock_service_init)
{
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    // Initialize service structure.
    p_lock_service->ble_input_write_handler    = p_lock_service_init->ble_input_write_handler;
    p_lock_service->latch_write_handler = p_lock_service_init->latch_write_handler;

    // Add service.
    ble_uuid128_t base_uuid = {LOCK_SERVICE_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_lock_service->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_lock_service->uuid_type;
    ble_uuid.uuid = LOCK_SERVICE_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_lock_service->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add BLE Input characteristic.
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = LOCK_SERVICE_UUID_BLE_INPUT_CHAR;
    add_char_params.uuid_type         = p_lock_service->uuid_type;
    add_char_params.init_len          = sizeof(uint8_t);
    add_char_params.max_len           = 20;
    add_char_params.is_var_len        = true;
    add_char_params.char_props.read   = 1;
    //add_char_params.char_props.notify = 1;
    add_char_params.char_props.write  = 1;

    add_char_params.read_access  = SEC_OPEN;
    add_char_params.write_access = SEC_OPEN;

    err_code = characteristic_add(p_lock_service->service_handle,
                                  &add_char_params,
                                  &p_lock_service->ble_input_char_handles);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }


    // Add Latch characteristic.
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = LOCK_SERVICE_UUID_LATCH_CHAR;
    add_char_params.uuid_type         = p_lock_service->uuid_type;
    add_char_params.init_len          = sizeof(uint8_t);
    add_char_params.max_len           = sizeof(uint8_t);
    add_char_params.char_props.read   = 1;
    //add_char_params.char_props.notify = 1;
    add_char_params.char_props.write  = 1;

    add_char_params.read_access  = SEC_OPEN;
    add_char_params.write_access = SEC_OPEN;

    return characteristic_add(p_lock_service->service_handle, &add_char_params, &p_lock_service->latch_char_handles);
}

//#endif // NRF_MODULE_ENABLED(BLE_LBS)
