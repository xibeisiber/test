// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class MinimalTimer : public rclcpp::Node {
public:
  MinimalTimer() : Node("minimal_timer") {
    timer_cb_group_ =
        this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    timer_ = create_wall_timer(
        500ms, std::bind(&MinimalTimer::timer_callback, this), timer_cb_group_);

    sub_cb_group_ =
        this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    options.callback_group = sub_cb_group_;
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "topic", 1,
        std::bind(&MinimalTimer::topic_callback, this, std::placeholders::_1),
        options);
  }

private:
  void timer_callback() {
    RCLCPP_INFO(this->get_logger(), "entering timer_callback");
    for (unsigned int i = 0; i < 100000000; i++) {
      float a = sqrt(i);
    }
    RCLCPP_INFO(this->get_logger(), "leaving timer_callback");
  }
  void topic_callback(const std_msgs::msg::String &msg) const {
    RCLCPP_INFO(this->get_logger(), "I heard---: '%s'", msg.data.c_str());
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::CallbackGroup::SharedPtr timer_cb_group_, sub_cb_group_;
  rclcpp::SubscriptionOptions options;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalTimer>());
  rclcpp::shutdown();
  return 0;
}