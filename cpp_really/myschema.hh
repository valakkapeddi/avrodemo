/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef MYSCHEMA_HH_2552856953__H_
#define MYSCHEMA_HH_2552856953__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace akkapeddi {
enum Side {
    BUY,
    SELL,
};

struct LimitOrder {
    std::string symbol;
    int32_t amount;
    double rate;
    Side side;
    LimitOrder() :
        symbol(std::string()),
        amount(int32_t()),
        rate(double()),
        side(Side())
        { }
};

}
namespace avro {
template<> struct codec_traits<akkapeddi::Side> {
    static void encode(Encoder& e, akkapeddi::Side v) {
		if (v < akkapeddi::BUY || v > akkapeddi::SELL)
		{
			std::ostringstream error;
			error << "enum value " << v << " is out of bound for akkapeddi::Side and cannot be encoded";
			throw avro::Exception(error.str());
		}
        e.encodeEnum(v);
    }
    static void decode(Decoder& d, akkapeddi::Side& v) {
		size_t index = d.decodeEnum();
		if (index < akkapeddi::BUY || index > akkapeddi::SELL)
		{
			std::ostringstream error;
			error << "enum value " << index << " is out of bound for akkapeddi::Side and cannot be decoded";
			throw avro::Exception(error.str());
		}
        v = static_cast<akkapeddi::Side>(index);
    }
};

template<> struct codec_traits<akkapeddi::LimitOrder> {
    static void encode(Encoder& e, const akkapeddi::LimitOrder& v) {
        avro::encode(e, v.symbol);
        avro::encode(e, v.amount);
        avro::encode(e, v.rate);
        avro::encode(e, v.side);
    }
    static void decode(Decoder& d, akkapeddi::LimitOrder& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.symbol);
                    break;
                case 1:
                    avro::decode(d, v.amount);
                    break;
                case 2:
                    avro::decode(d, v.rate);
                    break;
                case 3:
                    avro::decode(d, v.side);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.symbol);
            avro::decode(d, v.amount);
            avro::decode(d, v.rate);
            avro::decode(d, v.side);
        }
    }
};

}
#endif
