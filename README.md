# Bosch Sensortec's BMP280/BME280 Mongoose OS library

It uses Bosch Sensortec's reference implementation.

Both BMP280 and BME280 are supported with I2C or 4-wire SPI interface.

The library uses global instances of I2C or SPI with `spi.cs0_gpio`. 

If different pins than the default ones are used, the user should define them in the `config_schema` of `mos.yml`


