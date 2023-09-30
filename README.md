eeroOS Ethernet Interface Denial of Service Vulnerability
=========================================================

If eero Pro 5 devices receive an IPv6 packet where the payload length is zero
and contains a hop-by-hop or destination options extension header that refers
to another hop-by-hop or destination options extension header that doesn't
exist (because the packet ends before providing the content for it) then the
Ethernet interface will stop receiving any further packets. Transmission over
Ethernet, and all WiFi communication is unaffected.

All testing has been performed with a network of multiple eeros in "bridge"
mode. If a broadcast packet is used then all devices are affected at the same
time and there will be a complete loss of inbound Ethernet communication for
about 8 minutes until they automatically recover.

If a single eero is targeted then it will fallback to communicating with other
eeros over WiFi and start bridging all the traffic it receives out the Ethernet
interface (which is now transmit-only) with the corresponding problems that
occur with a bridged network loop. As long as it can still communicate with
other eeros it doesn't appear to automatically recover from this.

In both cases the effect is not permanent and the device can be rebooted to
recover immediately.

The packet can be unicast or multicast and does not need to be directed at the
eero's MAC or IPv6 address. In theory, if a unicast packet with these
characteristics could be routed over the Internet to an IPv6 host on the WiFi
network then it could be used to target eero devices remotely. In practise it
does not appear to be possible to get such a packet through a bridge/router
because it is malformed.

CVSS: 6.5 ([AV:A/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H/E:H/RL:U/RC:C](https://nvd.nist.gov/vuln-metrics/cvss/v3-calculator?vector=AV:A/AC:L/PR:N/UI:N/S:U/C:N/I:N/A:H/E:H/RL:U/RC:C&version=3.1))

Versions affected:
* eeroOS v6.9.0 and later

Timeline
--------
* 2022-08-05: Reported issue with eeroOS v6.9.0/eeroOS v6.10.3 on eero Pro 5 to eero
* 2022-08-06: Issue acknowledged
* 2022-10-09: Requested a timeline for when this will be fixed (no response)
* 2022-12-12: Received claim that it has been fixed in eeroOS v6.13
* 2022-12-12: Reported that issue remains with eeroOS v6.13.3-11 (no response)
* 2023-09-30: Confirmed issue is still present in v6.15.2-99
* 2023-09-30: Confirmed issue is still present in v6.16.4-11
* 2023-09-30: Public release
