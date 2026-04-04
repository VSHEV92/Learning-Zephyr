#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>

// set fench period to 500 msec
#define FETCH_PERIOD_MS 500

#define CORETEMP_NODE DT_NODELABEL(coretemp)
static const struct device *coretemp_dev = DEVICE_DT_GET(CORETEMP_NODE);

int main(void) {
    
    double temp;
    struct sensor_value sensor_data;

	while (true) {
        // sample sensor values
        sensor_sample_fetch(coretemp_dev);

        // get core temperature
        sensor_channel_get(coretemp_dev, SENSOR_CHAN_DIE_TEMP, &sensor_data);

        // convert sensor data to double format
        temp = sensor_value_to_double(&sensor_data);

        printk("Core temperature: %f\n", temp);
		k_msleep(FETCH_PERIOD_MS);
	}

	return 0;
}
