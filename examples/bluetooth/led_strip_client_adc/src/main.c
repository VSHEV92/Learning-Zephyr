#include <string.h>

#include <zephyr/kernel.h>

#include <zephyr/drivers/adc.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>

#define DEVICE_NAME "Esp32 Led Strip"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static const struct adc_dt_spec adc_spec = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

struct bt_conn *led_strip_conn;

// ---------------------------------------------
// ----       Scan and Connection           ----
// ---------------------------------------------
// advertising data parsing callback
static bool ad_parser_cb(struct bt_data *data, void *user_data) {
    bool* found_flag = (bool*)user_data;

    switch (data->type) {
        // try to find core temperature advertiser
        case BT_DATA_NAME_COMPLETE:
            if ( strncmp(data->data, DEVICE_NAME, DEVICE_NAME_LEN) == 0 ) {
                *found_flag = true;
                printk("Found led strip device\n");
                return true;
            }

        default: return true;
    }
}

// device found while scan callback
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad) {
    // check that device is connectable 
	if (type != BT_GAP_ADV_TYPE_ADV_IND && type != BT_GAP_ADV_TYPE_ADV_DIRECT_IND) return;

    // try to find device name in advertising data
    bool led_strip_found = false; 
    bt_data_parse(ad, ad_parser_cb, &led_strip_found);

    // exit if device not found
    if(!led_strip_found) return;

    // stop scanning and connect to device
	bt_le_scan_stop();
    bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN, BT_LE_CONN_PARAM_DEFAULT, &led_strip_conn);
}


// ---------------------------------------------
// ----        GATT Write Callback          ----
// ---------------------------------------------
static void write_hue_cb(struct bt_conn *conn, uint8_t err, struct bt_gatt_write_params *params) {
    printk("Write hue value: %s\n", (char*)params->data);
}


// ---------------------------------------------
// ---- Connection/disconnection callbcacks ----
// ---------------------------------------------
static void connected(struct bt_conn *conn, uint8_t conn_err) {
    printk("Connect to led strip device\n");
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("Disconnected with led strip device\n");
	bt_conn_unref(conn);
    bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

// ---------------------------------------------
// ----             Main Task               ----
// ---------------------------------------------
int main() {
    printk("Led Strip Client example\n");

    // ADC Configuration
    uint8_t hue_string[4];
    uint32_t hue_data;
    uint32_t hue_mask = (1 << adc_spec.resolution) - 1;

    struct adc_sequence sequence = {
		.buffer = &hue_data,
		.buffer_size = sizeof(hue_data),
	};

    adc_sequence_init_dt(&adc_spec, &sequence);
    adc_channel_setup_dt(&adc_spec);

    // Bluetoth init
    bt_enable(NULL);
    bt_le_scan_start(BT_LE_SCAN_ACTIVE, device_found);

    // gatt write parameters
    struct bt_gatt_write_params write_params = {
        .func = write_hue_cb,
        .handle = 0xb,
        .offset = 0,
        .data = hue_string,
        .length = sizeof(hue_string) - 1,
    };

    // main loop
    while(true) {
        
        k_msleep( 100 );

		if (led_strip_conn) {

            // get hue value from adc
            adc_read_dt(&adc_spec, &sequence);
            hue_data &= hue_mask;
            hue_data = (hue_data * 360.0) / (1 << adc_spec.resolution);

            // write hue
            snprintf((char*)hue_string, 4, "%3u", hue_data);
            bt_gatt_write(led_strip_conn, &write_params);
        }
    }

    return 0;
}
