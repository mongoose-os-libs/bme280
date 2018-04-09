#pragma once
#include "mgos_bme280.h"

class BME280 {
public:

  /*
   * Creates the BME280 object for the device with `addr` address
   */
  BME280(uint8_t addr, bool spi = false)
  : _bme(spi ? mgos_bme280_spi_create() : mgos_bme280_i2c_create(addr)) {
  }

  /*
   * Deletes the object and frees resources.
   */
  ~BME280() {
    mgos_bme280_delete(_bme);
  }

  /*
   * Reads the temperature, pressure and humidity in the provided `data` structure.
   * If the device is BMP280, the humidity will be 0.
   */
  int8_t read(struct mgos_bme280_data& data) {
    return mgos_bme280_read(_bme, &data);
  }

  /*
   * Reads the temperature.
   * Returns MGOS_BME280_ERROR if error.
   */
  double readTemperature() {
    return mgos_bme280_read_temperature(_bme);
  }

  /*
   * Reads the pressure.
   * Returns MGOS_BME280_ERROR if error.
   */
  double readPressure() {
    return mgos_bme280_read_pressure(_bme);
  }

  /*
   * Reads the humidity.
   * If the device is BMP280, the humidity will be 0.
   * Returns MGOS_BME280_ERROR if error.
   */
  double readHumidity() {
    return mgos_bme280_read_humidity(_bme);
  }

  /*
   * Returns true if a BME280 device is connected
   */
  bool isBME280() {
    return mgos_bme280_is_bme280(_bme);
  }

  bool getStats(struct mgos_bme280_stats *stats) {
    return mgos_bme280_getStats(_bme, stats);
  }
private:
  struct mgos_bme280* _bme;
};

