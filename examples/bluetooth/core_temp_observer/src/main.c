#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>

#define DEVICE_NAME "ESP32C6 Core Temp"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

// scan parameters structure 
static struct bt_le_scan_param scan_param = {
    .type       = BT_LE_SCAN_TYPE_ACTIVE,           // active (support scan request) or passive (not support them)
    .options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,  // filter duplicates, enable filter access list, enable coded-phy
    .interval   = BT_GAP_SCAN_FAST_INTERVAL_MIN,    // scan interval to 30 ms
    .window     = BT_GAP_SCAN_FAST_WINDOW,          // scan window to 30 ms
};

// advertising data parser callback
// data - advertising data element
// user - user data
static bool ad_parser_cb(struct bt_data *data, void *user_data) {
    char temp[5] = {0};
    bool* found_flag = (bool*)user_data;

    switch (data->type) {
        // try to find core temperature advertiser
        case BT_DATA_NAME_COMPLETE:
            if ( strncmp(data->data, DEVICE_NAME, DEVICE_NAME_LEN) == 0 ) {
                *found_flag = true;
                return true;
            }

        // print core temperature
        case BT_DATA_MANUFACTURER_DATA:
            if ( *found_flag ) {
                strncpy(temp, data->data + 2, 4);
                printk("Core temperature: %s\n", temp);
                }
            return true;

        default: return true;
    }
}


// scan callback
// addr - advertising device address
// rssi - signal strength in dBm
// type - type of advertizing (non-connectable, non-scannable, extended, periodic and so on)
// ad - buffer with advertisig data
static void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad) {
    
    bool core_temp_observer_found = false; 

    // parse advertising data
    bt_data_parse(
        ad,                        // advertising data buffer
        ad_parser_cb,              // parser callback
        &core_temp_observer_found  // user data
    );
}

int main() {

	bt_enable(NULL);

    bt_le_scan_start(&scan_param, scan_cb);
    
    return 0;
}
