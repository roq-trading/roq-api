/* Copyright (c) 2017,  Hans Erik Thrane */

#include <gtest/gtest.h>
#include "quinclas/codec.h"

using namespace flatbuffers;
using namespace quinclas;

TEST(flatbuffers, gateway_status) {
    auto source = common::GatewayStatusEvent{
        .message_info = {
            .gateway = "GATEWAY",
            .message_id = 123,
            .exchange_time = 4,
            .receive_time = 5,
            .enqueue_time = 6,
        },
        .gateway_status = {
            .market_data_connection_status = common::ConnectionStatus::Connected,
            .market_data_login_status = common::LoginStatus::On,
            .order_management_connection_status = common::ConnectionStatus::Connected,
            .order_management_login_status = common::LoginStatus::On,
        }
    };
    FlatBufferBuilder fbb(1024);
    ASSERT_EQ(fbb.GetSize(), 0);
    fbb.Finish(common::convert(fbb, source));
    ASSERT_GT(fbb.GetSize(), 0);
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());

    // FIXME: can't we specify max length?
    auto target = common::convert(GetRoot<schema::GatewayStatusEvent>(fbb.GetBufferPointer()));
    //auto target = common::convert(GetRoot<schema::GatewayStatusEvent>(&buffer[0]));
    ASSERT_EQ(target.gateway_status.market_data_connection_status, source.gateway_status.market_data_connection_status);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
