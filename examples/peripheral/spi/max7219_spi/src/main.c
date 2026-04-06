#include <zephyr/kernel.h>
#include <zephyr/drivers/spi.h>

#define SPI_NODE DT_NODELABEL(spi2)

static const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);

#define MAX7219_REG_INTENSITY		0x0A
#define MAX7219_REG_SCAN_LIMIT		0x0B
#define MAX7219_REG_SHUTDOWN		0x0C
#define MAX7219_REG_DISPLAY_TEST	0x0F

int main(void) {

    // buffer to display
    uint8_t display_data[] = {
        0b00000000,
        0b01101100,
        0b11111110,
        0b11111110,
        0b01111100,
        0b00111000,
        0b00010000,
        0b00000000
    };

    // spi buffers
    uint8_t addr;
    uint8_t data;

    struct spi_buf tx_buffs[] = {
        {.buf = &addr, .len = 1},
        {.buf = &data, .len = 1}
    };

    struct spi_buf_set tx_set = {.buffers = tx_buffs, .count = 2};

    // spi config
    struct spi_config spi_cfg = {
        .frequency = 10000000,
        .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
    };

    // disable test mode
    addr = MAX7219_REG_DISPLAY_TEST;
    data = 0;
    spi_write(spi_dev, &spi_cfg, &tx_set);

    // enable shutdown mode
    addr = MAX7219_REG_SHUTDOWN;
    data = 0;
    spi_write(spi_dev, &spi_cfg, &tx_set);

    // set scan limit
    addr = MAX7219_REG_SCAN_LIMIT;
    data = 0b00000111;
    spi_write(spi_dev, &spi_cfg, &tx_set);

    // set intensity
    addr = MAX7219_REG_INTENSITY;
    data = 0;
    spi_write(spi_dev, &spi_cfg, &tx_set);
    
    // write data
    for(int i = 0; i < 8; i++) {
        addr = i + 1;
        data = display_data[i];
        spi_write(spi_dev, &spi_cfg, &tx_set);
    }

    // disable shutdown mode
    addr = MAX7219_REG_SHUTDOWN;
    data = 1;
    spi_write(spi_dev, &spi_cfg, &tx_set);

    uint8_t intensity = 0;
    while(true) {
        addr = MAX7219_REG_INTENSITY;
        data = intensity;
        spi_write(spi_dev, &spi_cfg, &tx_set);

        intensity++;
        intensity %= 16;

        k_msleep(80);
    }
    
    return 0;
}
