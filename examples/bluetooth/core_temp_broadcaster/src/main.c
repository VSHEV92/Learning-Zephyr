#include <stdio.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/drivers/sensor.h>

#define CORETEMP_NODE DT_NODELABEL(coretemp)

#define DEVICE_NAME "ESP32C6 Core Temp"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

static const struct device *coretemp_dev = DEVICE_DT_GET(CORETEMP_NODE);

static uint8_t manifacture_data[7] = {0xE5, 0x02};

static const struct bt_data ad[] = {
	
    BT_DATA(
        BT_DATA_NAME_COMPLETE,
        DEVICE_NAME,    
        DEVICE_NAME_LEN
    ),

    BT_DATA(
        BT_DATA_MANUFACTURER_DATA,
        manifacture_data,    
        sizeof(manifacture_data) - 1
    ),
};


int main() {

	bt_enable(NULL);
	bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), NULL, 0);

    double temp;
    struct sensor_value sensor_data;

	while (true) {

        sensor_sample_fetch(coretemp_dev);
        sensor_channel_get(coretemp_dev, SENSOR_CHAN_DIE_TEMP, &sensor_data);
        temp = sensor_value_to_double(&sensor_data);

        sprintf(manifacture_data + 2, "%4.1f", temp);
        
        printk("Core temperature: %s\n", manifacture_data+2);

        bt_le_adv_update_data(ad, ARRAY_SIZE(ad), NULL, 0);
		
        k_msleep(500);
	}

	return 0;
}
