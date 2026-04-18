#include <gap.h>

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

static void connected(struct bt_conn *conn, uint8_t err) {
	char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("Connected to %s device\n", addr);
    led_strip_on();
}

static void disconnected(struct bt_conn *conn, uint8_t reason) {
	char addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));
	printk("Disconnected from %s device\n", addr);
    led_strip_off();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
    .recycled = start_advertising,
};
