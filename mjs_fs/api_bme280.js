
let BME280Data = {
    //data
    _crt_data: ffi('void* mgos_bme280_data_create()'),
    _dlt_data: ffi('void mgos_bme280_data_delete(void*)'),
    _temp: ffi('double mgos_bme280_data_get_temp(void*)'),
    _press: ffi('double mgos_bme280_data_get_press(void*)'),
    _humid: ffi('double mgos_bme280_data_get_humid(void*)'),

    // ## **`BME280Data.create()`**
    // Creates a BME280Data instance to be used for reading data from BME280.
    // Return value: an object with the methods described below.
    create: function () {
        let obj = Object.create(BME280Data._proto);
        obj.data = BME280Data._crt_data();
        return obj;
    },

    _proto: {
        // ## **`bmeData.free()`**
        // Frees a BME280Data instance.
        // No methods can be called on this instance after that.
        // Return value: none.
        free: function () {
            BME280Data._dlt_data(this.data);
        },

        // ## **`bmeData.temp()`**
        // Gets the temperature component of the BME280Data structure.
        temp: function () {
            return BME280Data._temp(this.data);
        },

        // ## **`bmeData.press()`**
        // Gets the pressure component of the BME280Data structure.
        press: function () {
            return BME280Data._press(this.data);
        },

        // ## **`bmeData.humid()`**
        // Gets the humidity component of the BME280Data structure.
        humid: function () {
            return BME280Data._humid(this.data);
        },
    },
};

let BME280 = {
    _crt_i2c: ffi('void* mgos_bme280_i2c_create(int)'),
    _crt_spi: ffi('void* mgos_bme280_spi_create()'),
    _dlt: ffi('void mgos_bme280_delete(void*)'),
    _read: ffi('int mgos_bme280_read(void*, void*)'),
    _temp: ffi('double mgos_bme280_read_temperature(void*)'),
    _press: ffi('double mgos_bme280_read_pressure(void*)'),
    _humid: ffi('double mgos_bme280_read_humidity(void*)'),

    MGOS_BME280_ERROR: -128.0,

    // ## **`BME280.createI2C(address)`**
    // Creates a BME280 instance on the I2C bus with the given address `address`.
    // Return value: an object with the methods described below.    
    createI2C: function (address) {
        let obj = Object.create(BME280._proto);
        obj.bme = BME280._crt_i2c(address);
        return obj;
    },

    // ## **`BME280.createSPI()`**
    // Creates a BME280 instance on the SPI bus, using `spi.cs0_gpio`
    // Return value: an object with the methods described below.    
    createSPI: function () {
        let obj = Object.create(BME280._proto);
        obj.bme = BME280._crt_spi();
        return obj;
    },

    _proto: {
        // ## **`bme.free()`**
        // Frees the  BME280 instance.
        // No methods can be called on this instance after that.
        // Return value: none.
        free: function () {
            BME280._dlt(this.bme);
        },

        // ## **`bme.readAll()`**
        // Reads all data into an instance of BME280Data
        // Returns zero if success, otherwise a negative value.
        readAll: function (data) {
            return BME280._read(this.bme, data.data);
        },

        // ## **`bme.readTemp()`**
        // Return the temperature from the sensor in degrees C or
        // `BME280.MGOS_BME280_ERROR` in case of a failure.
        readTemp: function () {
            return BME280._temp(this.bme);
        },

        // ## **`bme.readPress()`**
        // Return the pressure from the sensor in Pa
        // `BME280.MGOS_BME280_ERROR` in case of a failure.
        readPress: function () {
            return BME280._press(this.bme);
        },

        // ## **`bme.readHumid()`**
        // Return the humidity from the sensor in %RH
        // `BME280.MGOS_BME280_ERROR` in case of a failure.
        readHumid: function () {
            return BME280._humid(this.bme);
        },
    },
};