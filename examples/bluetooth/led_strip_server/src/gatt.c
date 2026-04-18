#include <gatt.h>

static ssize_t read_hue(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset) {
	const char *value = hue;
    printk("Read hue value: %s\n", value);
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}

static ssize_t write_hue(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
	uint8_t *value = hue;
	memcpy(value + offset, buf, len);
	value[offset + len] = 0;
    printk("Write hue value: %s\n", value);
    led_strip_update();
	return len;
}

static ssize_t write_hue_rotate(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags) {
    char c = ((uint8_t*)buf)[0];
	hue_rotate = c - '0';
    printk("Write hue rotate value: %d\n", (int)hue_rotate);
	return len;
}

static const struct bt_uuid_128 led_strip_svc_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0));

static const struct bt_uuid_128 hue_chr_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1));

static const struct bt_uuid_128 hue_rotate_chr_uuid = BT_UUID_INIT_128(
	BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef2));

BT_GATT_SERVICE_DEFINE(led_strip_svc,
	BT_GATT_PRIMARY_SERVICE(&led_strip_svc_uuid),
        BT_GATT_CHARACTERISTIC(
            &hue_chr_uuid.uuid,
            BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
            BT_GATT_PERM_READ | BT_GATT_PERM_WRITE,
            read_hue, 
            write_hue, 
            NULL
        ),
        BT_GATT_CHARACTERISTIC(
            &hue_rotate_chr_uuid.uuid,
            BT_GATT_CHRC_WRITE,
            BT_GATT_PERM_WRITE,
            NULL, 
            write_hue_rotate, 
            NULL
        ),
);
