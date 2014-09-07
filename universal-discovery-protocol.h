/*  -*- mode: c; c-basic-offset: 4; indent-tabs-mode: nil; -*-
 *  Universal Discovery Protocol
 *  A UDP protocol for finding Etherdream/Heroic Robotics lighting devices
 *
 *  (c) 2012 Jas Strong and Jacob Potter
 *  <jasmine@electronpusher.org> <jacobdp@gmail.com>
 */
#ifndef PIXELPUSHER_UNIVERSAL_DISCOVERY_PROTOCOL_H
#define PIXELPUSHER_UNIVERSAL_DISCOVERY_PROTOCOL_H

#include <stdint.h>

#define SFLAG_RGBOW (1<<0)
#define SFLAG_WIDEPIXELS (1<<1)
#define SFLAG_LOGARITHMIC (1<<2)
#define SFLAG_MOTION (1<<3)
#define SFLAG_NOTIDEMPOTENT (1<<4)

#define PFLAG_PROTECTED (1<<0)
#define PFLAG_FIXEDSIZE (1<<1)

typedef enum DeviceType {
    ETHERDREAM = 0,
    LUMIABRIDGE = 1,
    PIXELPUSHER = 2
} DeviceType;

typedef struct PixelPusher {
    uint8_t  strips_attached;
    uint8_t  max_strips_per_packet;
    uint16_t pixels_per_strip;  // uint16_t used to make alignment work
    uint32_t update_period; // in microseconds
    uint32_t power_total;   // in PWM units
    uint32_t delta_sequence;  // difference between received and expected sequence numbers
    int32_t controller_ordinal; // ordering number for this controller.
    int32_t group_ordinal;      // group number for this controller.
    uint16_t artnet_universe;   // configured artnet starting point for this controller
    uint16_t artnet_channel;
    uint16_t my_port;
    uint8_t strip_flags[8];     // flags for each strip, for up to eight strips
    uint32_t pusher_flags;      // flags for the whole pusher
    uint32_t segments;          // number of segments in each strip
    uint32_t power_domain;      // power domain of this pusher
    uint8_t last_driven_ip[4];  // last host to drive this pusher
    uint16_t last_driven_port;  // source port of last update
} PixelPusher;

typedef struct LumiaBridge {
    // placekeeper
} LumiaBridge;

typedef struct EtherDream {
    uint16_t buffer_capacity;
    uint32_t max_point_rate;
    uint8_t light_engine_state;
    uint8_t playback_state;
    uint8_t source;     //   0 = network
    uint16_t light_engine_flags;
    uint16_t playback_flags;
    uint16_t source_flags;
    uint16_t buffer_fullness;
    uint32_t point_rate;                // current point playback rate
    uint32_t point_count;           //  # points played
} EtherDream;

typedef union {
    PixelPusher pixelpusher;
    LumiaBridge lumiabridge;
    EtherDream etherdream;
} Particulars;

typedef struct DiscoveryPacketHeader {
    uint8_t mac_address[6];
    uint8_t ip_address[4];  // network byte order
    uint8_t device_type;
    uint8_t protocol_version; // for the device, not the discovery
    uint16_t vendor_id;
    uint16_t product_id;
    uint16_t hw_revision;
    uint16_t sw_revision;
    uint32_t link_speed;    // in bits per second
} DiscoveryPacketHeader;

typedef struct DiscoveryPacket {
    DiscoveryPacketHeader header;
    Particulars p;
} DiscoveryPacket;

#endif /* UNIVERSAL_DISCOVERY_PROTOCOL_H */
