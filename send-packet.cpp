#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <cstdio>
#include <cstring>
#include <memory>

int main(int argc, char *argv[]) {
	const char *ifname = argc >= 2 ? argv[1] : "eth0";

	int s = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
	if (s == -1) {
		std::perror("socket");
		return 1;
	}

	struct ifreq if_idx{};
	std::snprintf(if_idx.ifr_name, sizeof(if_idx.ifr_name), "%s", ifname);
	if (ioctl(s, SIOCGIFINDEX, &if_idx) < 0) {
		std::perror("ioctl(SIOCGIFINDEX)");
		return 1;
	}

	struct ifreq if_mac{};
	std::snprintf(if_mac.ifr_name, sizeof(if_idx.ifr_name), "%s", ifname);
	if (ioctl(s, SIOCGIFHWADDR, &if_mac) < 0) {
		std::perror("ioctl(SIOCGIFHWADDR)");
		return 1;
	}

	uint8_t data[] = {
		/* eth dst */
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		/* eth src */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* eth type */
		0x86, 0xdd,
		/* ipv6 */
		0x60, 0x00, 0x00, 0x00,
		/* ipv6 length */
		0x00, 0x00,
		/* ipv6 next header type (hop-by-hop options) */
		0x00,
		/* hop limit 64 */
		0x40,
		/* ipv6 src */
		0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		/* ipv6 dst */
		0x20, 0x01, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
		/* next header type (destination options) */
		0x3c,
		/* length */
		0x00,
		/* opts and padding */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};

	struct sockaddr_ll sa{};
	sa.sll_ifindex = if_idx.ifr_ifindex;
	sa.sll_halen = ETH_ALEN;
	std::memcpy(sa.sll_addr, data, ETH_ALEN);
	std::memcpy(&data[ETH_ALEN], if_mac.ifr_hwaddr.sa_data, ETH_ALEN);

	if (sendto(s, data, sizeof(data), 0, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
		std::perror("sendto");
		return 1;
	}

	return 0;
}
