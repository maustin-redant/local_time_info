#ifndef BLE_CTS_APP_H
#define BLE_CTS_APP_H

/** @file
 *
 * @defgroup bt_gatt_cts_c Current & Local Time Service Client
 * @{
 *
 * @brief Current & Local Time Service Client module.
 */

#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/conn.h>
#include <bluetooth/gatt_dm.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @def BT_UUID_CTS_LOCAL_TIME_VAL
 *  @brief CTS Characteristic Local Time UUID value
 */
#define BT_UUID_CTS_LOCAL_TIME_VAL		0x2A0F
/** @def BT_UUID_CTS_LOCAL_TIME
 *  @brief CTS Characteristic LOCAL Time
 */
#define BT_UUID_CTS_LOCAL_TIME \
	BT_UUID_DECLARE_16(BT_UUID_CTS_LOCAL_TIME_VAL)


/**@brief "Exact Time 256" field of the Current Time characteristic. */
struct bt_cts_exact_time_256 {
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t day_of_week;
	uint8_t fractions256;
};

/**@brief "Adjust Reason" field of the Current Time characteristic. */
struct bt_cts_adjust_reason {
	uint8_t manual_time_update : 1;
	uint8_t external_reference_time_update : 1;
	uint8_t change_of_time_zone : 1;
	uint8_t change_of_daylight_savings_time : 1;
};

/**@brief Data structure for the Current Time characteristic. */
struct bt_cts_current_time {
	struct bt_cts_exact_time_256 exact_time_256;
	struct bt_cts_adjust_reason adjust_reason;
};

/**
 * @brief CTS Local Time structure
 *
 * @param timeZone	Offset from UTC.  Value represents number of 15min increments
 * @param dst 		Daylight Savings offset. Value represents number of 15min increments
 */
struct bt_cts_local_time
{
    int8_t timeZone;           /**< Current Time Zone */
    uint8_t dst;               /**< Daylight Saving Time value */
} ;

struct bt_cts_client;

/**@brief Read complete callback.
 *
 * This function is called when the Current Time read operation finishes.
 *
 * @param cts_c         Current Time Service client structure.
 * @param current_time  The current time value that was read.
 * @param err           Error code.
 */
typedef void (*bt_cts_read_cb)(struct bt_cts_client *cts_c,
			       struct bt_cts_current_time *current_time,
			       int err);

/**@brief Value notification callback.
 *
 * This function is called every time the server sends a notification
 * for a changed value of the Current Time.
 *
 * @param cts_c         Current Time Service client structure.
 * @param current_time  The notified current time value.
 */
typedef void (*bt_cts_notify_cb)(struct bt_cts_client *cts_c,
				 struct bt_cts_current_time *current_time);

                 /**@brief Read complete callback.
 *
 * This function is called when the Local Time read operation finishes.
 *
 * @param cts_c         Local Time Service client structure.
 * @param current_time  The current time value that was read.
 * @param err           Error code.
 */
typedef void (*bt_lts_read_cb)(struct bt_cts_client *cts_c,
			       struct bt_cts_local_time *local_time,
			       int err);


/**@brief Current Time Service client structure. This structure contains status information for the client.*/
struct bt_cts_client {
	/** Connection object. */
	struct bt_conn *conn;

	/** Handle of the Current Time Characteristic. */
	uint16_t handle_ct;

    /** Handle of the Local Time Characteristic. */
	uint16_t handle_lt;

	/** Handle of the CCCD of the Current Time Characteristic. */
	uint16_t handle_ct_ccc;

	/** Internal state. */
	atomic_t state;

	/** Read parameters. */
	struct bt_gatt_read_params read_params;

	/** Notification parameters. */
	struct bt_gatt_subscribe_params notify_params;

	/** Current Time Read value callback. */
	bt_cts_read_cb read_ct_cb;

	/** Current Time Notification callback. */
	bt_cts_notify_cb notify_ct_cb;

    /** Local Time Read value callback. */
	bt_lts_read_cb read_lt_cb;
};


/**@brief Function for initializing the Current Time Service client.
 *
 * @param[out] cts_c  Current Time Service client structure. This structure must
 *                    be supplied by the application. It is initialized by this
 *                    function and can later be used to identify this particular client
 *                    instance.
 *
 * @retval 0 If the client was initialized successfully.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_client_init(struct bt_cts_client *cts_c);

/**@brief Function for reading the peer's Current Time Service Current Time characteristic.
 *
 * @param[in] cts_c  Current Time Service client structure.
 * @param[in] func   The callback function.
 *
 * @retval 0 If all operations are successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_read_current_time(struct bt_cts_client *cts_c, bt_cts_read_cb func);


/**
 * @brief Subscribe to the current time value change notification.
 *
 * @param[in] cts_c  Current Time Service client structure.
 * @param[in] func   The callback function.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_subscribe_current_time(struct bt_cts_client *cts_c,
				  bt_cts_notify_cb func);

/**
 * @brief Remove the subscription.
 *
 * @param[in] cts_c  Current Time Service client structure.
 *
 * @retval 0 If the operation was successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_unsubscribe_current_time(struct bt_cts_client *cts_c);

/**@brief Function for reading the peer's Local Time Service Current Time characteristic.
 *
 * @param[in] cts_c  Local Time Service client structure.
 * @param[in] func   The callback function.
 *
 * @retval 0 If all operations are successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_read_local_time(struct bt_cts_client *cts_c, bt_lts_read_cb func);


/**@brief Function for assigning handles to this instance of cts_c.
 *
 * @details Call this function when a link has been established with a peer to
 *          associate the link to this instance of the module. This makes it
 *          possible to handle several links and associate each link to a particular
 *          instance of this module.
 *
 * @param[in]  dm     Discovery object.
 * @param[out] cts_c  Current Time Service client structure.
 *
 * @retval 0 If the operation is successful.
 *           Otherwise, a (negative) error code is returned.
 */
int bt_cts_handles_assign(struct bt_gatt_dm *dm, struct bt_cts_client *cts_c);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* BLE_CTS_APP_H */
