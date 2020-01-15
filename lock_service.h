#ifndef BLE_LOCK_SERVICE_H__
#define BLE_LOCK_SERVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#define BLE_LOCK_SERVICE_BLE_OBSERVER_PRIO 2

#ifdef __cplusplus
extern "C" {
#endif

/**@brief   Macro for defining a ble_lock_service instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_LOCK_SERVICE_DEF(_name)                                                                          \
static ble_lock_service_t _name;                                                                             \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                          \
                     BLE_LOCK_SERVICE_BLE_OBSERVER_PRIO,                                                     \
                     ble_lock_service_on_ble_evt, &_name)

#define LOCK_SERVICE_UUID_BASE        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define LOCK_SERVICE_UUID_SERVICE            0x0001
#define LOCK_SERVICE_UUID_BLE_INPUT_CHAR     0x0002
#define LOCK_SERVICE_UUID_LATCH_CHAR         0x0003

// Forward declaration of the ble_lock_service_t type.
typedef struct ble_lock_service_s ble_lock_service_t;

typedef void (*ble_lock_service_ble_input_write_handler_t) (uint16_t conn_handle, ble_lock_service_t * p_lock_service, uint8_t * ble_input, uint16_t length);

typedef void (*ble_lock_service_latch_write_handler_t) (uint16_t conn_handle, ble_lock_service_t * p_lock_service, uint8_t new_state);

/** @brief Lock Service init structure. This structure contains all options and data needed for
 *        initialization of the service.*/
typedef struct
{
    ble_lock_service_ble_input_write_handler_t     ble_input_write_handler;    /**< Event handler to be called when the BLE Input Characteristic is written. */
    ble_lock_service_latch_write_handler_t         latch_write_handler; /**< Event handler to be called when the Latch Characteristic is written. */
} ble_lock_service_init_t;


/**@brief Lock Service structure. This structure contains various status information for the service. */
struct ble_lock_service_s
{
    uint16_t                                      service_handle;             /**< Handle of Lock Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t                      ble_input_char_handles;     /**< Handles related to the Password Characteristic. */
    ble_gatts_char_handles_t                      latch_char_handles;         /**< Handles related to the Latch Characteristic. */
    uint8_t                                       uuid_type;                  /**< UUID type for the Lock Service. */
    ble_lock_service_ble_input_write_handler_t    ble_input_write_handler;           /**< Event handler to be called when the Password Characteristic is written. */
    ble_lock_service_latch_write_handler_t        latch_write_handler;        /**< Event handler to be called when the Latch Characteristic is written. */
};


/**@brief Function for initializing the Lock Service.
 *
 * @param[out] p_lock_service      Lock Service structure. This structure must be supplied by
 *                                 the application. It is initialized by this function and will later
 *                                 be used to identify this particular service instance.
 * @param[in] p_lock_service_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_lock_service_init(ble_lock_service_t * p_lock_service, const ble_lock_service_init_t * p_lock_service_init);


/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the Lock Service.
 *
 * @param[in] p_ble_evt  Event received from the BLE stack.
 * @param[in] p_context  Lock Service structure.
 */
void ble_lock_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

#ifdef __cplusplus
}
#endif

#endif // BLE_LOCK_SERVICE_H__

/** @} */
