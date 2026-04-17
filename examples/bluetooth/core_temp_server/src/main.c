#include <stdio.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include <zephyr/drivers/sensor.h>

#define CORETEMP_NODE DT_NODELABEL(coretemp)
static const struct device *coretemp_dev = DEVICE_DT_GET(CORETEMP_NODE);

static uint8_t core_temp_value[4 + 1] = {0};
static bool core_temp_indication = false;

// GATT core temperature read callback
static ssize_t read_core_temp(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
	const char *value = attr->user_data;
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}

// GATT core temperature indication callback
static void core_temp_ccc_cfg_changed(const struct bt_gatt_attr *attr, uint16_t value) {
	if (value == BT_GATT_CCC_INDICATE) {
        core_temp_indication = true;
    }
    else {
        core_temp_indication = false;
    }
}


// Create core temperature UUIDs
static const struct bt_uuid_128 core_temp_svc_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0));

static const struct bt_uuid_128 core_temp_chr_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1));


// Core temperature Primary Service
BT_GATT_SERVICE_DEFINE(core_temp_svc,
	BT_GATT_PRIMARY_SERVICE(&core_temp_svc_uuid),
	BT_GATT_CHARACTERISTIC(
        &core_temp_chr_uuid.uuid,
		BT_GATT_CHRC_READ | BT_GATT_CHRC_INDICATE,
		BT_GATT_PERM_READ,
		read_core_temp, 
        NULL, 
        core_temp_value
    ),
	BT_GATT_CCC(
        core_temp_ccc_cfg_changed,
		BT_GATT_PERM_READ | BT_GATT_PERM_WRITE
    ),
);


// advertising data
static const struct bt_data ad[] = {
    BT_DATA(
        BT_DATA_NAME_COMPLETE,
        CONFIG_BT_DEVICE_NAME,    
        (sizeof(CONFIG_BT_DEVICE_NAME) - 1)
    ),
};

void start_advertising() {
	bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), NULL, 0);
}


// define callbacks for connectionm disaconnction, security changes and parameters changes
static void connected(struct bt_conn *conn, uint8_t err) {
	char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("Connected to %s device\n", addr);
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
	char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("Disconnected from %s device\n", addr);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
    .recycled = start_advertising,
};



int main() {

    // init bluetoth and start advertising
	bt_enable(NULL);
    start_advertising();

    
    // get core temperature attribute
	struct bt_gatt_attr *core_temp_attr = bt_gatt_find_by_uuid(
        core_temp_svc.attrs, 
        core_temp_svc.attr_count, 
        &core_temp_chr_uuid.uuid
    );


    // read core temperature
	while (true) {
        double temp;
        struct sensor_value sensor_data;

        // read core temperature
        sensor_sample_fetch(coretemp_dev);
        sensor_channel_get(coretemp_dev, SENSOR_CHAN_DIE_TEMP, &sensor_data);
        temp = sensor_value_to_double(&sensor_data);

        sprintf(core_temp_value, "%4.1f", temp);

		// Core temperature indication 
		if (core_temp_indication) {
            struct bt_gatt_indicate_params ind_params = {
                .attr = core_temp_attr,
                .func = NULL,
                .destroy = NULL,
                .data = core_temp_value,
                .len = sizeof(core_temp_value),
            };
			bt_gatt_indicate(NULL, &ind_params); 

            printk("Send indication. Core temperature: %s\n", core_temp_value);
        }

        k_msleep(500);
	}

	return 0;
}
