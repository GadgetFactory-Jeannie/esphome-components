#include "math.h"

#include "qwiic_joystick.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace qwiic_joystick {

static const char *const TAG = "qwiic_joystick";


void QwiicJoystick::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Joystick...");
  
  uint16_t version = 0;
  
  if( this->read_bytes_16(0x01, &version, 1) )
    ESP_LOGCONFIG(TAG, "- Version %d.", version);
  else
    ESP_LOGCONFIG(TAG, "- Error: could not read version number!");
}

void QwiicJoystick::update() {
  uint8_t button_pressed, button_clicked;
  
  uint8_t x_msb, x_lsb, y_msb, y_lsb;
  
  this->read_bytes(0x07, &button_pressed, 1);
  this->read_bytes(0x08, &button_clicked, 1);
  
  this->read_bytes(0x03, &x_msb, 1);
  this->read_bytes(0x04, &x_lsb, 1);
  
  this->read_bytes(0x05, &y_msb, 1);
  this->read_bytes(0x06, &y_lsb, 1);
  
  uint16_t x = ((x_msb << 8) | x_lsb) >> 6;
  uint16_t y = ((y_msb << 8) | y_lsb) >> 6;
  
  int16_t x_msb_c = static_cast<int16_t>(x_msb)-128;
  int16_t y_msb_c = static_cast<int16_t>(y_msb)-128;
  
  
  if( this->button_sensor_ ) {
    this->button_sensor_->publish_state(static_cast<bool>button_clicked);
    this->button_sensor_->publish_state(static_cast<bool>button_pressed);
  }  

  if( this->x_axis_raw_sensor_ )
    this->x_axis_raw_sensor_->publish_state(x);
  
  if( this->x_axis_raw_centered_sensor_ )
    this->x_axis_raw_sensor_->publish_state(static_cast<int16_t>(x)-512);
  
  if( this->x_axis_8_bit_sensor_ )
    this->x_axis_raw_sensor_->publish_state(x_msb);
  
  if( this->x_axis_8_bit_centered_sensor_ )
    this->x_axis_raw_sensor_->publish_state(x_msb_c);
  
  if( this->x_axis_percent_sensor_ )
    this->x_axis_percent_sensor_->publish_state(static_cast<double>(x)/1024.0);
    
  if( this->x_axis_percent_centered_sensor_ )
    this->x_axis_percent_centered_sensor_->publish_state(static_cast<double>(x)/512.0 - 100.0);
    
  
  if( this->y_axis_raw_sensor_ )
    this->y_axis_raw_sensor_->publish_state(y);
  
  if( this->y_axis_raw_centered_sensor_ )
    this->y_axis_raw_sensor_->publish_state(static_cast<int16_t>(y)-512);
  
  if( this->y_axis_8_bit_sensor_ )
    this->y_axis_raw_sensor_->publish_state(y_msb);
  
  if( this->y_axis_8_bit_centered_sensor_ )
    this->y_axis_raw_sensor_->publish_state(y_msb_c);
  
  if( this->y_axis_percent_sensor_ )
    this->y_axis_percent_sensor_->publish_state(static_cast<double>(y)/1024.0);
    
  if( this->y_axis_percent_centered_sensor_ )
    this->y_axis_percent_centered_sensor_->publish_state(static_cast<double>(y)/512.0 - 100.0);
  
    
  if( this->radius_squared_raw_sensor_ )
    this->radius_squared_raw_sensor_->publish_state(x_msb_c*x_msb_c+y_msb_c*y_msb_c);
  
  if( this->radius_raw_sensor_ )
    this->radius_raw_sensor_->publish_state(sqrt(x_msb_c*x_msb_c+y_msb_c*y_msb_c));
  
  if( this->radius_squared_percent_sensor_ )
    this->radius_squared_percent_sensor_->publish_state(static_cast<double>(x_msb_c*x_msb_c+y_msb_c*y_msb_c)/65536.0);
  
  if( this->radius_percent_sensor_ )
    this->radius_percent_sensor_->publish_state(sqrt(static_cast<double>(x_msb_c*x_msb_c+y_msb_c*y_msb_c))/65536.0);
  
  
  if( this->theta_degrees_sensor_ )
    this->theta_degrees_sensor_->publish_state(atan2(static_cast<double>(y), static_cast<double>(x))*180.0/3.14159);
    
  if( this->theta_radians_sensor_ )
    this->theta_radians_sensor_->publish_state(atan2(static_cast<double>(y), static_cast<double>(x)));
    
  
  
}

float QwiicJoystick::get_setup_priority() const {
  return setup_priority::IO;
}



} // namespace qwiic_joystick
} // namespace esphome
