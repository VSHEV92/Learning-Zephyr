#include <zephyr/bluetooth/bluetooth.h>

#define DEVICE_NAME "ESP32C6 Beacon"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/* 
 * BT_LE_ADV_NCONN - macro that init struct bt_le_adv_param to non-connectable 
 *                   advertizing with private (random) address
 *
 * BT_DATA - helper to create bt_data struct. Need specify type, data and data len in bytes
 *
 * BT_DATA_BYTES - helper to create bt_data struct. Need specify only type and data
 *
 * BT_BYTES_LIST_LE16 - macro convert 16-bit value to little-endian array of bytes
 *
 */


// create advertising data (type, data_len, data)
static const struct bt_data ad[] = {
    
    // flags
	BT_DATA_BYTES(
        BT_DATA_FLAGS,                        // type 0x1
        BT_LE_AD_NO_BREDR | BT_LE_AD_GENERAL  // set flags to general discoverable mode, no classic (BR/EDR) mode
    ),
	
    // device name
    BT_DATA(
        BT_DATA_NAME_COMPLETE, // type 0x9
        DEVICE_NAME,    
        DEVICE_NAME_LEN
    ),
};


// create scan response data (type, data_len, data)
static const struct bt_data sd[] = {
    // set device appearence
	BT_DATA_BYTES(
        BT_DATA_GAP_APPEARANCE,                       // type 0x19
        BT_BYTES_LIST_LE16(BT_APPEARANCE_GENERIC_TAG) // appearence to generic tag - 0x200
    ),
};


int main() {

    // initialize bluetooth stack
	bt_enable(
        NULL  // callback that run after init, if NULL call is blocking
    );


    // start advertizing
	bt_le_adv_start(
        BT_LE_ADV_NCONN, // Advertising parameters, struct bt_le_adv_param
        ad,              // advertizing data, array of bt_data (type, len, data)
        ARRAY_SIZE(ad),  // advertising data size
        sd,              // scan response data, array of bt_data
        ARRAY_SIZE(sd)   // scan response data size
    );

	return 0;
}
