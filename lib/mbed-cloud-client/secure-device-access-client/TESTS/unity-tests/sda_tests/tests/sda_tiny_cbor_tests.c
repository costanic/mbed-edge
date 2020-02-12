// ----------------------------------------------------------------------------
// Copyright 2017-2019 ARM Ltd.
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
// ----------------------------------------------------------------------------


#include "unity_fixture.h"
#include "sda_log.h"
#include "cose.h"
#include "cose_int.h"
#include "sda_internal_defs.h"
#include "secure_device_access.h"
#include "factory_configurator_client.h"
#include "sda_status.h"
#include "sda_bundle_parser.h"
#include "pal.h"
#include "test_utils.h"
#include "sda_malloc.h"
#include "sda_trust_anchor.h"
#include "sda_verification.h"
#include "tinycbor.h"

/* These tests check added tiny cbor functionality,
 The tests implemented in sda and not in tiny cbor , as we didn't want to add new test to qt test platform*/

TEST_GROUP(sda_cbor_test);

TEST_SETUP(sda_cbor_test)
{
}


TEST_TEAR_DOWN(sda_cbor_test)
{

}

CborType expected_types_array[9 * 2] =
{
    CborIntegerType /*12*/,
    CborTextStringType /*"lcd-display-hello" */,
    CborIntegerType /*25*/,
    CborMapType /*{1: {1: 2, -1: 1, -2: h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27',-3: h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}}*/,
    CborIntegerType /*2*/,
    CborTextStringType /* "015e5237de0902420a013e0c00000000"*/,
    CborIntegerType /*3*/,
    CborArrayType /*["ep:0160c1976ccf0a580a010e1303c00000"]*/,
    CborIntegerType /*4*/,
    CborIntegerType /*1522101892*/,
    CborIntegerType /*5*/,
    CborIntegerType /*1520892292*/,
    CborIntegerType /*6*/,
    CborIntegerType /*1520892292*/,
    CborIntegerType /*1*/,
    CborTextStringType /*"mbed.ta.F1387949AE943143B52F2C09D4763D65DF269C47AE80567ED6B14218F53F3DB0"*/,
    CborIntegerType /*7*/,
    CborByteStringType /* h'01621C3EAC1A0A580A01129500000000'*/,
};

uint8_t  encoded_complex_map[] =
{
    /*
    {12: "lcd-display-hello",
    25: {1: {1: 2, -1: 1, -2: h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27',-3: h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}},
    2: "015e5237de0902420a013e0c00000000",
    3: ["ep:0160c1976ccf0a580a010e1303c00000"],
    4: 1522101892,
    5: 1520892292,
    6: 1520892292,
    1: "mbed.ta.F1387949AE943143B52F2C09D4763D65DF269C47AE80567ED6B14218F53F3DB0",
    7: h'01621C3EAC1A0A580A01129500000000'}
    */
    0xA9, 0x0C, 0x71, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x2D, 0x68, 0x65, 0x6C, 0x6C
    , 0x6F, 0x18, 0x19, 0xA1, 0x01, 0xA4, 0x01, 0x02, 0x20, 0x01, 0x21, 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07
    , 0xA0, 0xED, 0xC7, 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26, 0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF
    , 0x1E, 0xE2, 0xFA, 0x0A, 0x9B, 0xCB, 0x27, 0x22, 0x58, 0x20, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F
    , 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC, 0x66, 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6
    , 0x3B, 0xC4, 0x56, 0x06, 0x02, 0x78, 0x20, 0x30, 0x31, 0x35, 0x65, 0x35, 0x32, 0x33, 0x37, 0x64, 0x65, 0x30, 0x39
    , 0x30, 0x32, 0x34, 0x32, 0x30, 0x61, 0x30, 0x31, 0x33, 0x65, 0x30, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
    , 0x30, 0x03, 0x81, 0x78, 0x23, 0x65, 0x70, 0x3A, 0x30, 0x31, 0x36, 0x30, 0x63, 0x31, 0x39, 0x37, 0x36, 0x63, 0x63
    , 0x66, 0x30, 0x61, 0x35, 0x38, 0x30, 0x61, 0x30, 0x31, 0x30, 0x65, 0x31, 0x33, 0x30, 0x33, 0x63, 0x30, 0x30, 0x30
    , 0x30, 0x30, 0x04, 0x1A, 0x5A, 0xB9, 0x6E, 0x84, 0x05, 0x1A, 0x5A, 0xA6, 0xF9, 0x84, 0x06, 0x1A, 0x5A, 0xA6, 0xF9
    , 0x84, 0x01, 0x78, 0x48, 0x6D, 0x62, 0x65, 0x64, 0x2E, 0x74, 0x61, 0x2E, 0x46, 0x31, 0x33, 0x38, 0x37, 0x39, 0x34
    , 0x39, 0x41, 0x45, 0x39, 0x34, 0x33, 0x31, 0x34, 0x33, 0x42, 0x35, 0x32, 0x46, 0x32, 0x43, 0x30, 0x39, 0x44, 0x34
    , 0x37, 0x36, 0x33, 0x44, 0x36, 0x35, 0x44, 0x46, 0x32, 0x36, 0x39, 0x43, 0x34, 0x37, 0x41, 0x45, 0x38, 0x30, 0x35
    , 0x36, 0x37, 0x45, 0x44, 0x36, 0x42, 0x31, 0x34, 0x32, 0x31, 0x38, 0x46, 0x35, 0x33, 0x46, 0x33, 0x44, 0x42, 0x30
    , 0x07, 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC, 0x1A, 0x0A, 0x58, 0x0A, 0x01, 0x12, 0x95, 0x00, 0x00, 0x00, 0x00
};

TEST(sda_cbor_test, map_container_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue map_value;
    CborValue map_start_container;
    int num_of_types = 9 * 2;
    int index = 0;

    cbor_err = cbor_parser_init((const uint8_t*)encoded_complex_map, sizeof(encoded_complex_map), 0, &cbor_parser, &map_value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(map_value.type, CborMapType);

    cbor_err = cbor_init_container(&map_value, &map_start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(map_start_container.type, CborIntegerType);//12

    for (index = 1; index < num_of_types; index++) {
        cbor_err = cbor_get_next_container_element(&map_start_container);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);
        TEST_ASSERT_EQUAL(map_start_container.type, expected_types_array[index]);
    }

    cbor_err = cbor_get_next_container_element(&map_start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorAdvancePastEOF);

}

TEST(sda_cbor_test, array_container_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborValue start_container;
    int num_of_types = 9 * 2;
    int index = 0;
    uint8_t  encoded_complex_array[] =
    {   
        /* 
        [12, "lcd-display-hello",
        25, {1: {1: 2, -1: 1, -2: h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27', -3: h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}},
        2, "015e5237de0902420a013e0c00000000", 
        3, ["ep:0160c1976ccf0a580a010e1303c00000"],
        4, 1522101892,
        5, 1520892292,
        6, 1520892292,
        1, "mbed.ta.F1387949AE943143B52F2C09D4763D65DF269C47AE80567ED6B14218F53F3DB0",
        7, h'01621C3EAC1A0A580A01129500000000']
        */
        0x92, 0x0C, 0x71, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x2D, 0x68, 0x65, 0x6C, 0x6C
        , 0x6F, 0x18, 0x19, 0xA1, 0x01, 0xA4, 0x01, 0x02, 0x20, 0x01, 0x21, 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07
        , 0xA0, 0xED, 0xC7, 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26, 0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF
        , 0x1E, 0xE2, 0xFA, 0x0A, 0x9B, 0xCB, 0x27, 0x22, 0x58, 0x20, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F
        , 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC, 0x66, 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6
        , 0x3B, 0xC4, 0x56, 0x06, 0x02, 0x78, 0x20, 0x30, 0x31, 0x35, 0x65, 0x35, 0x32, 0x33, 0x37, 0x64, 0x65, 0x30, 0x39
        , 0x30, 0x32, 0x34, 0x32, 0x30, 0x61, 0x30, 0x31, 0x33, 0x65, 0x30, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
        , 0x30, 0x03, 0x81, 0x78, 0x23, 0x65, 0x70, 0x3A, 0x30, 0x31, 0x36, 0x30, 0x63, 0x31, 0x39, 0x37, 0x36, 0x63, 0x63
        , 0x66, 0x30, 0x61, 0x35, 0x38, 0x30, 0x61, 0x30, 0x31, 0x30, 0x65, 0x31, 0x33, 0x30, 0x33, 0x63, 0x30, 0x30, 0x30
        , 0x30, 0x30, 0x04, 0x1A, 0x5A, 0xB9, 0x6E, 0x84, 0x05, 0x1A, 0x5A, 0xA6, 0xF9, 0x84, 0x06, 0x1A, 0x5A, 0xA6, 0xF9
        , 0x84, 0x01, 0x78, 0x48, 0x6D, 0x62, 0x65, 0x64, 0x2E, 0x74, 0x61, 0x2E, 0x46, 0x31, 0x33, 0x38, 0x37, 0x39, 0x34
        , 0x39, 0x41, 0x45, 0x39, 0x34, 0x33, 0x31, 0x34, 0x33, 0x42, 0x35, 0x32, 0x46, 0x32, 0x43, 0x30, 0x39, 0x44, 0x34
        , 0x37, 0x36, 0x33, 0x44, 0x36, 0x35, 0x44, 0x46, 0x32, 0x36, 0x39, 0x43, 0x34, 0x37, 0x41, 0x45, 0x38, 0x30, 0x35
        , 0x36, 0x37, 0x45, 0x44, 0x36, 0x42, 0x31, 0x34, 0x32, 0x31, 0x38, 0x46, 0x35, 0x33, 0x46, 0x33, 0x44, 0x42, 0x30
        , 0x07, 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC, 0x1A, 0x0A, 0x58, 0x0A, 0x01, 0x12, 0x95, 0x00, 0x00, 0x00, 0x00
    };

    cbor_err = cbor_parser_init((const uint8_t*)encoded_complex_array, sizeof(encoded_complex_array), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(value.type, CborArrayType);

    cbor_err = cbor_init_container(&value, &start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(start_container.type, CborIntegerType);//12

    for (index = 1; index < num_of_types; index++) {
        cbor_err = cbor_get_next_container_element(&start_container);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);
        TEST_ASSERT_EQUAL(start_container.type, expected_types_array[index]);
    }

    cbor_err = cbor_get_next_container_element(&start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorAdvancePastEOF);

}

TEST(sda_cbor_test, mixed_container_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborValue start_container;
    int num_of_types =8;
    int index = 0;
    CborType expected_mixed_types_array[8] =
    {

        CborMapType /*{5: "lcd-display-hello"}*/,
        CborIntegerType /*7*/,
        CborByteStringType /* h'01621C3EAC1A0A580A01129500000000'*/,
        CborTagType, /*18()*/
        CborArrayType,/*[1,2,3]*/
        CborDoubleType, /*7.2*/
        CborIntegerType, /*0*/
        CborBooleanType /*true*/
    };

    uint8_t  encoded_complex_map[] =
    {
        /*
        [ {5: "lcd-display-hello"} , 
        7, 
        h'01621C3EAC1A0A580A01129500000000', 
        18([1,2,3])],
        7.2, 
        0, 
        true]
        */
        0x87, 0xA1, 0x05, 0x71, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x2D, 0x68
        , 0x65, 0x6C, 0x6C, 0x6F, 0x07, 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC, 0x1A, 0x0A, 0x58, 0x0A, 0x01, 0x12, 0x95, 0x00
        , 0x00, 0x00, 0x00, 0xD2, 0x83, 0x01, 0x02, 0x03, 0xFB, 0x40, 0x1C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCD, 0x00, 0xF5
    };

    cbor_err = cbor_parser_init((const uint8_t*)encoded_complex_map, sizeof(encoded_complex_map), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(value.type, CborArrayType);

    cbor_err = cbor_init_container(&value, &start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(start_container.type, CborMapType);

    for (index = 1; index < num_of_types; index++) {
        cbor_err = cbor_get_next_container_element(&start_container);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);
        TEST_ASSERT_EQUAL(start_container.type, expected_mixed_types_array[index]);
    }

    cbor_err = cbor_get_next_container_element(&start_container);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorAdvancePastEOF);
}

TEST(sda_cbor_test, fixed_container_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborValue start_container;
    int num_of_types = 8;
    int index = 0;
    uint8_t  simple_value_1[] =  {0x05};
    uint8_t  simple_value_2[] = { 0x64,0x74,0x65,0x78,0x74};
    uint8_t  simple_value_3[] = { 0x21 };
    uint8_t *temp = NULL;
    size_t temp_size = 0;
    CborType expected_type = CborInvalidType;

    for (index = 0; index < 3; index++)
    {
        if (index == 0) {
            temp = simple_value_1;
            temp_size = sizeof(simple_value_1);
            expected_type = CborIntegerType;
        }
        if (index == 1) {
            temp = simple_value_2;
            temp_size = sizeof(simple_value_2);
            expected_type = CborTextStringType;
        }
        if (index == 2) {
            temp = simple_value_3;
            temp_size = sizeof(simple_value_3);
            expected_type = CborIntegerType;
        }
        cbor_err = cbor_parser_init((const uint8_t*)temp, temp_size, 0, &cbor_parser, &value);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);

        cbor_err = cbor_init_container(&value, &start_container);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);
        TEST_ASSERT_EQUAL(start_container.type, expected_type);

        cbor_err = cbor_get_next_container_element(&start_container);
        TEST_ASSERT_EQUAL(cbor_err, CborErrorAdvancePastEOF);
    }

}

TEST(sda_cbor_test, get_array_element_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborValue array_element;
    CborValue start_container;
    int num_of_types = 8;
    int index = 0;
    uint8_t *out_buffer = NULL;
    size_t out_buffer_size = 0;
    uint8_t array_1[] = { 0x80 };
    uint8_t array_2[] = {
        /* [12] */
        0x81, 0x0c};
    uint8_t  array_3[] = {
           /*
           [664396747115767313,  // 1st element
           [1, "lcd-display",["hello"]], //2nd element
           h'D83DD28443A10126A059011CA90C716C63642D646973706C61792D68656C6C6F1819A101A401022001215820534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB272258202E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC4560602782030313565353233376465303930323432306130313365306330303030303030300381782365703A3031363063313937366363663061353830613031306531333033633030303030041A5AB96E84051A5AA6F984061A5AA6F9840178486D6265642E74612E46313338373934394145393433313433423532463243303944343736334436354446323639433437414538303536374544364231343231384635334633444230075001621C3EAC1A0A580A011295000000005840B65D28B6EAA306F4427C3BCD9726692285375C00BEEB17EBFF4CF078E0848A3D77889FDADEA3F75A98884814C71904FDF4198568530FDC754C680C8ABE452176']
           //3rd element*/
        0x83, 0x1B, 0x09, 0x38, 0x69, 0x52, 0x45, 0xC2, 0xA6, 0x11, 0x83, 0x01, 0x6B, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69
        , 0x73, 0x70, 0x6C, 0x61, 0x79, 0x81, 0x65, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x59, 0x01, 0x6A, 0xD8, 0x3D, 0xD2, 0x84
        , 0x43, 0xA1, 0x01, 0x26, 0xA0, 0x59, 0x01, 0x1C, 0xA9, 0x0C, 0x71, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70
        , 0x6C, 0x61, 0x79, 0x2D, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x18, 0x19, 0xA1, 0x01, 0xA4, 0x01, 0x02, 0x20, 0x01, 0x21
        , 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07, 0xA0, 0xED, 0xC7, 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26, 0x21, 0x36
        , 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF, 0x1E, 0xE2, 0xFA, 0x0A, 0x9B, 0xCB, 0x27, 0x22, 0x58, 0x20, 0x2E
        , 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC, 0x66, 0x0B, 0x77, 0x1F, 0x2E
        , 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56, 0x06, 0x02, 0x78, 0x20, 0x30, 0x31, 0x35, 0x65
        , 0x35, 0x32, 0x33, 0x37, 0x64, 0x65, 0x30, 0x39, 0x30, 0x32, 0x34, 0x32, 0x30, 0x61, 0x30, 0x31, 0x33, 0x65, 0x30
        , 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x03, 0x81, 0x78, 0x23, 0x65, 0x70, 0x3A, 0x30, 0x31, 0x36
        , 0x30, 0x63, 0x31, 0x39, 0x37, 0x36, 0x63, 0x63, 0x66, 0x30, 0x61, 0x35, 0x38, 0x30, 0x61, 0x30, 0x31, 0x30, 0x65
        , 0x31, 0x33, 0x30, 0x33, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x04, 0x1A, 0x5A, 0xB9, 0x6E, 0x84, 0x05, 0x1A, 0x5A
        , 0xA6, 0xF9, 0x84, 0x06, 0x1A, 0x5A, 0xA6, 0xF9, 0x84, 0x01, 0x78, 0x48, 0x6D, 0x62, 0x65, 0x64, 0x2E, 0x74, 0x61
        , 0x2E, 0x46, 0x31, 0x33, 0x38, 0x37, 0x39, 0x34, 0x39, 0x41, 0x45, 0x39, 0x34, 0x33, 0x31, 0x34, 0x33, 0x42, 0x35
        , 0x32, 0x46, 0x32, 0x43, 0x30, 0x39, 0x44, 0x34, 0x37, 0x36, 0x33, 0x44, 0x36, 0x35, 0x44, 0x46, 0x32, 0x36, 0x39
        , 0x43, 0x34, 0x37, 0x41, 0x45, 0x38, 0x30, 0x35, 0x36, 0x37, 0x45, 0x44, 0x36, 0x42, 0x31, 0x34, 0x32, 0x31, 0x38
        , 0x46, 0x35, 0x33, 0x46, 0x33, 0x44, 0x42, 0x30, 0x07, 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC, 0x1A, 0x0A, 0x58, 0x0A
        , 0x01, 0x12, 0x95, 0x00, 0x00, 0x00, 0x00, 0x58, 0x40, 0xB6, 0x5D, 0x28, 0xB6, 0xEA, 0xA3, 0x06, 0xF4, 0x42, 0x7C
        , 0x3B, 0xCD, 0x97, 0x26, 0x69, 0x22, 0x85, 0x37, 0x5C, 0x00, 0xBE, 0xEB, 0x17, 0xEB, 0xFF, 0x4C, 0xF0, 0x78, 0xE0
        , 0x84, 0x8A, 0x3D, 0x77, 0x88, 0x9F, 0xDA, 0xDE, 0xA3, 0xF7, 0x5A, 0x98, 0x88, 0x48, 0x14, 0xC7, 0x19, 0x04, 0xFD
        , 0xF4, 0x19, 0x85, 0x68, 0x53, 0x0F, 0xDC, 0x75, 0x4C, 0x68, 0x0C, 0x8A, 0xBE, 0x45, 0x21, 0x76
    };

    uint8_t  array_3_second_element[] = {
        0x83, 0x01, 0x6B, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x81, 0x65, 0x68, 0x65, 0x6C
        , 0x6C, 0x6F
    };

    uint8_t  array_3_third_element[] = {
        0xD8, 0x3D, 0xD2, 0x84, 0x43, 0xA1, 0x01, 0x26, 0xA0, 0x59, 0x01, 0x1C, 0xA9, 0x0C, 0x71, 0x6C, 0x63, 0x64, 0x2D
        , 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x2D, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x18, 0x19, 0xA1, 0x01, 0xA4, 0x01
        , 0x02, 0x20, 0x01, 0x21, 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07, 0xA0, 0xED, 0xC7, 0x50, 0x5F, 0x4D, 0x75
        , 0xE4, 0x26, 0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF, 0x1E, 0xE2, 0xFA, 0x0A, 0x9B, 0xCB, 0x27
        , 0x22, 0x58, 0x20, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC, 0x66
        , 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56, 0x06, 0x02, 0x78, 0x20
        , 0x30, 0x31, 0x35, 0x65, 0x35, 0x32, 0x33, 0x37, 0x64, 0x65, 0x30, 0x39, 0x30, 0x32, 0x34, 0x32, 0x30, 0x61, 0x30
        , 0x31, 0x33, 0x65, 0x30, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x03, 0x81, 0x78, 0x23, 0x65, 0x70
        , 0x3A, 0x30, 0x31, 0x36, 0x30, 0x63, 0x31, 0x39, 0x37, 0x36, 0x63, 0x63, 0x66, 0x30, 0x61, 0x35, 0x38, 0x30, 0x61
        , 0x30, 0x31, 0x30, 0x65, 0x31, 0x33, 0x30, 0x33, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x04, 0x1A, 0x5A, 0xB9, 0x6E
        , 0x84, 0x05, 0x1A, 0x5A, 0xA6, 0xF9, 0x84, 0x06, 0x1A, 0x5A, 0xA6, 0xF9, 0x84, 0x01, 0x78, 0x48, 0x6D, 0x62, 0x65
        , 0x64, 0x2E, 0x74, 0x61, 0x2E, 0x46, 0x31, 0x33, 0x38, 0x37, 0x39, 0x34, 0x39, 0x41, 0x45, 0x39, 0x34, 0x33, 0x31
        , 0x34, 0x33, 0x42, 0x35, 0x32, 0x46, 0x32, 0x43, 0x30, 0x39, 0x44, 0x34, 0x37, 0x36, 0x33, 0x44, 0x36, 0x35, 0x44
        , 0x46, 0x32, 0x36, 0x39, 0x43, 0x34, 0x37, 0x41, 0x45, 0x38, 0x30, 0x35, 0x36, 0x37, 0x45, 0x44, 0x36, 0x42, 0x31
        , 0x34, 0x32, 0x31, 0x38, 0x46, 0x35, 0x33, 0x46, 0x33, 0x44, 0x42, 0x30, 0x07, 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC
        , 0x1A, 0x0A, 0x58, 0x0A, 0x01, 0x12, 0x95, 0x00, 0x00, 0x00, 0x00, 0x58, 0x40, 0xB6, 0x5D, 0x28, 0xB6, 0xEA, 0xA3
        , 0x06, 0xF4, 0x42, 0x7C, 0x3B, 0xCD, 0x97, 0x26, 0x69, 0x22, 0x85, 0x37, 0x5C, 0x00, 0xBE, 0xEB, 0x17, 0xEB, 0xFF
        , 0x4C, 0xF0, 0x78, 0xE0, 0x84, 0x8A, 0x3D, 0x77, 0x88, 0x9F, 0xDA, 0xDE, 0xA3, 0xF7, 0x5A, 0x98, 0x88, 0x48, 0x14
        , 0xC7, 0x19, 0x04, 0xFD, 0xF4, 0x19, 0x85, 0x68, 0x53, 0x0F, 0xDC, 0x75, 0x4C, 0x68, 0x0C, 0x8A, 0xBE, 0x45, 0x21
        , 0x76
    };

    uint64_t uint_value = 0;
    int int_value = 0;

    //Check first array , the array is empty
    cbor_err = cbor_parser_init((const uint8_t*)array_1, sizeof(array_1), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    cbor_err = cbor_get_array_element(&value, 0, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);
    /*****************************************************************************************************/

    //Check second array , one element in the array
    cbor_err = cbor_parser_init((const uint8_t*)array_2, sizeof(array_2), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    cbor_err = cbor_get_array_element(&value, 0, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(array_element.type, CborIntegerType);

    cbor_err = cbor_value_get_int(&array_element, &int_value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(int_value, 12);

    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);
    /*****************************************************************************************************/

    //Check third array ,three element in the array
    cbor_err = cbor_parser_init((const uint8_t*)array_3, sizeof(array_3), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    //Check first element
    /**********************/
    cbor_err = cbor_get_array_element(&value, 0, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(array_element.type, CborIntegerType);

    cbor_err = cbor_value_get_uint64(&array_element, &uint_value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(uint_value, 664396747115767313);

    //check second element
    /**********************/
    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(array_element.type, CborArrayType);
    TEST_ASSERT_EQUAL_INT8_ARRAY(array_element.ptr, array_3_second_element, sizeof(array_3_second_element));

    //Check third element
    /**********************/
    cbor_err = cbor_get_array_element(&value, 2, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(array_element.type, CborByteStringType);

    cbor_value_get_byte_string_chunk(&array_element, &out_buffer, &out_buffer_size, NULL);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(out_buffer_size, sizeof(array_3_third_element));
    TEST_ASSERT_EQUAL_INT8_ARRAY(out_buffer, array_3_third_element, out_buffer_size);

    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(array_element.type, CborArrayType);
    TEST_ASSERT_EQUAL_INT8_ARRAY(array_element.ptr, array_3_second_element, sizeof(array_3_second_element));


    //Check twice
    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(array_element.type, CborArrayType);
    TEST_ASSERT_EQUAL_INT8_ARRAY(array_element.ptr, array_3_second_element, sizeof(array_3_second_element));

    //Bad cases
    cbor_err = cbor_get_array_element(&value, 3, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

    cbor_err = cbor_get_array_element(NULL, 0, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

    cbor_err = cbor_get_array_element(&value, 0, NULL);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);
    
    //Check non array element
    cbor_err = cbor_parser_init((const uint8_t*)array_3_third_element, sizeof(array_3_third_element), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    cbor_err = cbor_get_array_element(&value, 1, &array_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);
}

int array_of_keys[9] = { 25,2,3,4,5,6,1,7,12 };
uint8_t key_value_1[] = 
{ 0x71, 0x6C, 0x63, 0x64, 0x2D, 0x64, 0x69, 0x73, 0x70, 0x6C, 0x61, 0x79, 0x2D, 0x68, 0x65, 0x6C, 0x6C, 0x6F 
};/* "lcd-display-hello"    -> key 12*/

uint8_t key_value_2[] =
{ 0xA1, 0x01, 0xA4, 0x01, 0x02, 0x20, 0x01, 0x21, 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07, 0xA0, 0xED, 0xC7
, 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26, 0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF, 0x1E, 0xE2, 0xFA
, 0x0A, 0x9B, 0xCB, 0x27, 0x22, 0x58, 0x20, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83
, 0x38, 0x9A, 0xCC, 0x66, 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56
, 0x06
}; /* {1: {1: 2, -1: 1, -2: h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27', -3: h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}}     -> key 25*/

uint8_t key_value_3[] = { 0x78, 0x20, 0x30, 0x31, 0x35, 0x65, 0x35, 0x32, 0x33, 0x37, 0x64, 0x65, 0x30, 0x39, 0x30, 0x32, 0x34, 0x32, 0x30
, 0x61, 0x30, 0x31, 0x33, 0x65, 0x30, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30
};/* "015e5237de0902420a013e0c00000000"     -> key 2 */

uint8_t key_value_4[] =
{
    0x81, 0x78, 0x23, 0x65, 0x70, 0x3A, 0x30, 0x31, 0x36, 0x30, 0x63, 0x31, 0x39, 0x37, 0x36, 0x63, 0x63, 0x66, 0x30
    , 0x61, 0x35, 0x38, 0x30, 0x61, 0x30, 0x31, 0x30, 0x65, 0x31, 0x33, 0x30, 0x33, 0x63, 0x30, 0x30, 0x30, 0x30, 0x30
};/* ["ep:0160c1976ccf0a580a010e1303c00000"]     -> key 3*/

uint8_t key_value_5[] = { 0x1A, 0x5A, 0xB9, 0x6E, 0x84 };/* 1522101892     -> key 4*/
uint8_t key_value_6[] = { 0x1A, 0x5A,0xA6, 0xF9, 0x84 };/* 1520892292     -> key 5*/
uint8_t key_value_7[] = { 0x1A, 0x5A,0xA6, 0xF9, 0x84 };/* 1520892292     -> key 6*/
uint8_t key_value_8[] =
{
     0x78, 0x48, 0x6D, 0x62, 0x65, 0x64, 0x2E, 0x74, 0x61, 0x2E, 0x46, 0x31, 0x33, 0x38, 0x37, 0x39, 0x34, 0x39, 0x41
    , 0x45, 0x39, 0x34, 0x33, 0x31, 0x34, 0x33, 0x42, 0x35, 0x32, 0x46, 0x32, 0x43, 0x30, 0x39, 0x44, 0x34, 0x37, 0x36
    , 0x33, 0x44, 0x36, 0x35, 0x44, 0x46, 0x32, 0x36, 0x39, 0x43, 0x34, 0x37, 0x41, 0x45, 0x38, 0x30, 0x35, 0x36, 0x37
    , 0x45, 0x44, 0x36, 0x42, 0x31, 0x34, 0x32, 0x31, 0x38, 0x46, 0x35, 0x33, 0x46, 0x33, 0x44, 0x42, 0x30

};/* "mbed.ta.F1387949AE943143B52F2C09D4763D65DF269C47AE80567ED6B14218F53F3DB0"      -> key 1*/

uint8_t key_value_9[] =
{ 0x50, 0x01, 0x62, 0x1C, 0x3E, 0xAC, 0x1A, 0x0A, 0x58, 0x0A, 0x01, 0x12, 0x95, 0x00, 0x00, 0x00, 0x00
};/* h'01621C3EAC1A0A580A01129500000000'     -> key 7*/


typedef struct cbor_value_data_ {
    /** The error, or CN_CBOR_NO_ERROR if none */
    uint8_t *buffer;
    size_t buffer_size;
} cbor_value_data_s;


TEST(sda_cbor_test, get_map_element_test)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborParser cbor_parser_1;
    CborValue value_1;
    CborValue map_element;
    CborValue start_container;
    int num_of_types = 8;
    int index = 0;
    uint8_t *out_buffer = NULL;
    size_t out_buffer_size = 0;
    int test_index = 0;
    cbor_value_data_s array_of_values[] = {

        {
            .buffer = key_value_2,
            .buffer_size = sizeof(key_value_2)
        },
        {
            .buffer = key_value_3,
            .buffer_size = sizeof(key_value_3)
        },
        {
            .buffer = key_value_4,
            .buffer_size = sizeof(key_value_4)
        },
        {
            .buffer = key_value_5,
            .buffer_size = sizeof(key_value_5)
        },
        {
            .buffer = key_value_6,
            .buffer_size = sizeof(key_value_6)
        },
        {
            .buffer = key_value_7,
            .buffer_size = sizeof(key_value_7)
        },
        {
            .buffer = key_value_8,
            .buffer_size = sizeof(key_value_8)
        },
        {
            .buffer = key_value_9,
            .buffer_size = sizeof(key_value_9)
        },
        {
            .buffer = key_value_1 ,
            .buffer_size = sizeof(key_value_1)
        },
    };

    //Check first array , the array is empty
    cbor_err = cbor_parser_init((const uint8_t*)encoded_complex_map, sizeof(encoded_complex_map), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    for (test_index = 0; test_index < 9; test_index++) {
        cbor_err = cbor_get_map_element_by_int_key(&value, array_of_keys[test_index], &map_element);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);

        //parse value buffers for the iteration and compare with the result of map_element
        cbor_err = cbor_parser_init((const uint8_t*)array_of_values[test_index].buffer, array_of_values[test_index].buffer_size, 0, &cbor_parser_1, &value_1);
        TEST_ASSERT_EQUAL(cbor_err, CborNoError);
        TEST_ASSERT_EQUAL_INT8_ARRAY(value_1.ptr, map_element.ptr, array_of_values[test_index].buffer_size);
    }

    //Bad cases
    cbor_err = cbor_get_map_element_by_int_key(&value, 50, &map_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

    //Check good index after failure
    cbor_err = cbor_get_map_element_by_int_key(&value, 4, &map_element);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    TEST_ASSERT_EQUAL(map_element.type, CborIntegerType);
    TEST_ASSERT_EQUAL_INT8_ARRAY(map_element.ptr, array_of_values[3].buffer, array_of_values[3].buffer_size);

    //Bad params
    cbor_err = cbor_get_map_element_by_int_key(NULL, 4, &map_element);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

    cbor_err = cbor_get_map_element_by_int_key(&value, 4, NULL);
    TEST_ASSERT_EQUAL(cbor_err, CborErrorIO);

}

TEST(sda_cbor_test, cose_key_test)
{
    CborError cbor_err = CborNoError;
    uint8_t pk[KCM_EC_SECP256R1_MAX_PUB_KEY_RAW_SIZE];

    uint8_t expected_pk[KCM_EC_SECP256R1_MAX_PUB_KEY_RAW_SIZE] = { 
          0x04, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07, 0xA0, 0xED, 0xC7, 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26,
          0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF, 0x1E, 0xE2, 0xFA, 0x0A, 0x9B, 0xCB,
          0x27, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC,
          0x66, 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56,
          0x06 };
    size_t pk_size_out = 0;
    bool status = false;
    cose_errback cose_err;

    uint8_t encrypted_cose_buffer[] =
    {    0xA4, 0x01, 0x02, 0x20, 0x01, 0x21, 0x58, 0x20, 0x53, 0x4C, 0xBA, 0x8D, 0xA8, 0x07, 0xA0, 0xED, 0xC7
        , 0x50, 0x5F, 0x4D, 0x75, 0xE4, 0x26, 0x21, 0x36, 0x5B, 0xC6, 0xBA, 0xB2, 0xBC, 0xA6, 0xA3, 0xBF, 0x1E, 0xE2, 0xFA
        , 0x0A, 0x9B, 0xCB, 0x27, 0x22, 0x58, 0x20, 0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83
        , 0x38, 0x9A, 0xCC, 0x66, 0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56
        , 0x06
    };



    status =  GetECKeyFromCoseBuffer(encrypted_cose_buffer, sizeof(encrypted_cose_buffer), pk, sizeof(pk), &pk_size_out, &cose_err);
    TEST_ASSERT_EQUAL(status, true);
    TEST_ASSERT_EQUAL(pk_size_out, KCM_EC_SECP256R1_MAX_PUB_KEY_RAW_SIZE);
    TEST_ASSERT_EQUAL_INT8_ARRAY(pk, expected_pk, KCM_EC_SECP256R1_MAX_PUB_KEY_RAW_SIZE);

}

TEST(sda_cbor_test, get_data_buffer_from_cbor_value)
{
    CborError cbor_err = CborNoError;
    CborParser cbor_parser;
    CborValue value;
    CborValue map_value;
    CborValue byte_map_value;
    int num_of_types = 8;
    int index = 0;
    uint8_t *out_buffer = NULL;
    size_t *out_buffer_size = 0;

    uint8_t expected_data[] = {
        0x58, 0x20,
        0x2E, 0x9D, 0x5E, 0xC0, 0x93, 0x4C, 0x91, 0xF9, 0x1F, 0x3B, 0x14, 0x83, 0x38, 0x9A, 0xCC, 0x66,
        0x0B, 0x77, 0x1F, 0x2E, 0x61, 0x3A, 0xD6, 0xE3, 0x6C, 0xA9, 0x68, 0xD6, 0x3B, 0xC4, 0x56, 0x06
    };

    cbor_err = cbor_parser_init((const uint8_t*)key_value_2, sizeof(key_value_2), 0, &cbor_parser, &value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    //{1: {1: 2, -1 : 1, -2 : h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27', -3 : h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}}
    cbor_err = cbor_get_cbor_payload_buffer_in_container(&value, &out_buffer, &out_buffer_size);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    //The value data should be the same as original buffer
    TEST_ASSERT_EQUAL(sizeof(key_value_2), out_buffer_size);
    TEST_ASSERT_EQUAL_INT8_ARRAY(out_buffer, key_value_2, out_buffer_size);

    //{1: 2, -1 : 1, -2 : h'534CBA8DA807A0EDC7505F4D75E42621365BC6BAB2BCA6A3BF1EE2FA0A9BCB27', -3 : h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'}
    cbor_err = cbor_get_map_element_by_int_key(&value,1, &map_value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    out_buffer = NULL;
    out_buffer_size = 0;

    cbor_err = cbor_get_cbor_payload_buffer_in_container(&map_value, &out_buffer, &out_buffer_size);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    //The value data is the internal map value, starts from offset 2
    TEST_ASSERT_EQUAL(sizeof(key_value_2) - 2, out_buffer_size);
    TEST_ASSERT_EQUAL_INT8_ARRAY(out_buffer, key_value_2 + 2, out_buffer_size);


    //h'2E9D5EC0934C91F91F3B1483389ACC660B771F2E613AD6E36CA968D63BC45606'
    cbor_err = cbor_get_map_element_by_int_key(&map_value, -3, &byte_map_value);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);

    out_buffer = NULL;
    out_buffer_size = 0;

    cbor_err = cbor_get_cbor_payload_buffer_in_container(&byte_map_value, &out_buffer, &out_buffer_size);
    TEST_ASSERT_EQUAL(cbor_err, CborNoError);
    //The value data is the internal map value, starts from offset 2
    TEST_ASSERT_EQUAL(sizeof(expected_data) , out_buffer_size);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected_data, out_buffer, out_buffer_size);

}



TEST_GROUP_RUNNER(sda_cbor_test)
{
    //positive tests
    RUN_TEST_CASE(sda_cbor_test, map_container_test);
    RUN_TEST_CASE(sda_cbor_test, array_container_test);
    RUN_TEST_CASE(sda_cbor_test, mixed_container_test);
    RUN_TEST_CASE(sda_cbor_test, fixed_container_test);
    RUN_TEST_CASE(sda_cbor_test, get_array_element_test);
    RUN_TEST_CASE(sda_cbor_test, get_map_element_test);
    RUN_TEST_CASE(sda_cbor_test, cose_key_test);
    RUN_TEST_CASE(sda_cbor_test, get_data_buffer_from_cbor_value);

}