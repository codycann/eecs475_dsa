/**
 * Test cases generated using speck implementation in python:
 * https://github.com/bozhu/NSA-ciphers/blob/master/speck.py
 *
 * Not guaranteed to be correct, but serves as a good cross-comparison.
 */
static const speck_test_vector test_vectors[] = {
	{32, 64, {0xf3, 0x1e, 0x9e, 0x08, 0x26, 0x25, 0xc1, 0xa0}, {0x1d, 0x42, 0xe9, 0xe4}, {0x92, 0x0f, 0x1e, 0xba}},
	{32, 64, {0xa3, 0xae, 0x42, 0x4d, 0xe9, 0xd1, 0x20, 0xad}, {0xff, 0x86, 0xd8, 0x22}, {0x90, 0xfe, 0x32, 0xdf}},
	{32, 64, {0x79, 0x33, 0x9f, 0xb2, 0x2e, 0xbb, 0xc7, 0x17}, {0xec, 0x49, 0x10, 0x7e}, {0x1d, 0xd3, 0xf0, 0x24}},
	{32, 64, {0xc8, 0x11, 0x1f, 0x5f, 0x16, 0x04, 0x18, 0x91}, {0x24, 0x25, 0xb9, 0x31}, {0x4a, 0xda, 0x67, 0xad}},
	{32, 64, {0xb2, 0xe9, 0x36, 0xe0, 0x8f, 0x9b, 0xb5, 0x6c}, {0x69, 0x1b, 0xc4, 0xbf}, {0x49, 0x31, 0x1e, 0x0e}},
	{32, 64, {0x91, 0x88, 0xd3, 0x03, 0x5a, 0x3f, 0x87, 0x20}, {0x29, 0xc9, 0x36, 0x22}, {0x0e, 0x91, 0xf3, 0x19}},
	{32, 64, {0x99, 0xfb, 0xbb, 0x4b, 0x42, 0xde, 0x0a, 0xb5}, {0x3c, 0x43, 0x67, 0x23}, {0x40, 0x14, 0x11, 0x78}},
	{32, 64, {0xfc, 0x45, 0x52, 0x5a, 0xcd, 0xc2, 0x4e, 0x5c}, {0x45, 0x9d, 0x8f, 0x7c}, {0x82, 0x29, 0x45, 0x89}},
	{32, 64, {0xc4, 0x33, 0xa5, 0xa6, 0x8d, 0x4e, 0x89, 0x5b}, {0xce, 0x2a, 0xc5, 0x43}, {0x7b, 0x49, 0xb2, 0xd0}},
	{32, 64, {0x56, 0xb6, 0xb3, 0x07, 0x7b, 0x55, 0x85, 0xe6}, {0x40, 0x31, 0x5b, 0x7c}, {0xe5, 0x11, 0x50, 0x30}},
	{48, 72, {0x54, 0x9d, 0xa7, 0x58, 0xe6, 0xbd, 0x33, 0xd5, 0xf8}, {0xd9, 0x21, 0x78, 0x60, 0x5d, 0x71}, {0x11, 0x0c, 0x2a, 0x0e, 0x12, 0xbb}},
	{48, 72, {0x3b, 0x7b, 0x32, 0x79, 0x37, 0xe0, 0x22, 0x63, 0x7f}, {0x31, 0x81, 0xa3, 0x1e, 0x47, 0xf3}, {0x5f, 0x15, 0x62, 0x28, 0x95, 0x67}},
	{48, 72, {0xde, 0xe9, 0x59, 0x57, 0x93, 0x5d, 0xb7, 0x9b, 0xa0}, {0xb1, 0x93, 0xf8, 0x89, 0x04, 0xf5}, {0x16, 0xf3, 0xed, 0x10, 0x20, 0x0d}},
	{48, 72, {0x51, 0x5b, 0x8c, 0x67, 0x5d, 0xc4, 0x12, 0x11, 0xac}, {0xc1, 0xea, 0xf1, 0xf5, 0x79, 0x64}, {0xdb, 0xd3, 0x2e, 0x66, 0x07, 0xd9}},
	{48, 72, {0x92, 0x7c, 0x3f, 0x28, 0xc4, 0x3b, 0x15, 0x28, 0x9e}, {0x06, 0xa2, 0x92, 0x94, 0xa4, 0x67}, {0x22, 0x1f, 0xa0, 0xeb, 0x9f, 0x93}},
	{48, 72, {0xd5, 0x68, 0x14, 0x8c, 0xe9, 0x96, 0xdf, 0x95, 0xaf}, {0x22, 0xcb, 0xf7, 0x64, 0x12, 0x3c}, {0xe5, 0x8a, 0xd6, 0x0e, 0x92, 0xb0}},
	{48, 72, {0x77, 0xc4, 0x16, 0xbc, 0x98, 0x90, 0xab, 0x43, 0x21}, {0x7d, 0x0f, 0x53, 0x7f, 0xa4, 0x64}, {0x46, 0xf6, 0x61, 0x20, 0x45, 0xd3}},
	{48, 72, {0xdf, 0x07, 0x8d, 0xdd, 0x50, 0x78, 0x87, 0x65, 0xcb}, {0x31, 0xdc, 0x75, 0x39, 0x80, 0xf5}, {0x64, 0xb8, 0xe8, 0x4b, 0xaa, 0xae}},
	{48, 72, {0x35, 0x8b, 0x0e, 0x40, 0x29, 0x8e, 0x01, 0x87, 0xee}, {0x82, 0x5b, 0xe0, 0x7b, 0xa5, 0x97}, {0x05, 0x6e, 0x01, 0x3d, 0xee, 0x1e}},
	{48, 72, {0x74, 0x7e, 0xeb, 0x17, 0xad, 0xdd, 0x93, 0x6d, 0x0c}, {0x0d, 0x7c, 0x33, 0xf5, 0x86, 0xd2}, {0x91, 0x4f, 0x03, 0x81, 0xc1, 0x50}},
	{48, 96, {0xc1, 0x13, 0x31, 0xcb, 0x16, 0x62, 0x2b, 0xc3, 0x8d, 0xc1, 0x4b, 0xfb}, {0xb2, 0x9f, 0x90, 0x5e, 0xb2, 0xf9}, {0x12, 0x12, 0x22, 0x98, 0x2f, 0x8d}},
	{48, 96, {0x07, 0x15, 0xd7, 0x34, 0xa9, 0x7e, 0x5a, 0x8d, 0x1e, 0xb3, 0x11, 0x87}, {0x35, 0x9e, 0x66, 0x72, 0xc9, 0x3a}, {0x0b, 0x1a, 0xd8, 0xe4, 0xd6, 0x43}},
	{48, 96, {0xe4, 0x0b, 0x8c, 0xcf, 0xa4, 0x30, 0xfc, 0xbd, 0x37, 0x61, 0xbf, 0x92}, {0x44, 0x98, 0x01, 0x9d, 0xb0, 0x77}, {0x82, 0x5b, 0x1e, 0xff, 0x21, 0xed}},
	{48, 96, {0xa2, 0x71, 0x37, 0xfe, 0xa9, 0x8f, 0xb7, 0x37, 0x12, 0x64, 0x0a, 0xc6}, {0xd9, 0x76, 0x4a, 0xf2, 0x5a, 0xc7}, {0x14, 0x7a, 0xa9, 0x42, 0xee, 0x86}},
	{48, 96, {0xcb, 0x7d, 0x14, 0x8d, 0xe3, 0x3b, 0x25, 0x94, 0xee, 0x93, 0x28, 0x13}, {0xe3, 0x90, 0x22, 0xb3, 0x44, 0x32}, {0xc3, 0x67, 0x02, 0x33, 0x6e, 0xf7}},
	{48, 96, {0x7d, 0x74, 0xf7, 0x64, 0xbe, 0x04, 0xda, 0x12, 0x7d, 0xc3, 0xae, 0xbf}, {0x35, 0x34, 0xfb, 0x03, 0x70, 0x91}, {0x7a, 0x04, 0x38, 0x3c, 0xee, 0x77}},
	{48, 96, {0x02, 0xc4, 0x29, 0xb3, 0xc4, 0x84, 0x75, 0xb2, 0x3f, 0x93, 0x00, 0x9c}, {0x1c, 0xc5, 0x90, 0x05, 0x17, 0x52}, {0xee, 0x45, 0xba, 0xec, 0xb7, 0xeb}},
	{48, 96, {0x07, 0x62, 0x50, 0x7a, 0xc4, 0x1c, 0x1d, 0xb0, 0x66, 0x6f, 0x40, 0xdb}, {0x6f, 0x4c, 0x6d, 0x04, 0x19, 0x9a}, {0x68, 0xb4, 0x31, 0x7c, 0xd2, 0x8c}},
	{48, 96, {0x55, 0x60, 0x12, 0x74, 0x8f, 0x91, 0xa9, 0x21, 0xeb, 0xe6, 0x47, 0x72}, {0x46, 0x6a, 0x7e, 0xc5, 0xe8, 0x13}, {0x10, 0xd1, 0xf0, 0x6e, 0x4c, 0xae}},
	{48, 96, {0x06, 0x59, 0x7f, 0x21, 0x7f, 0xc3, 0x2e, 0x8f, 0x62, 0x5a, 0x63, 0x5f}, {0x92, 0xae, 0xad, 0x7c, 0x14, 0x2c}, {0xb0, 0xbe, 0xda, 0x8d, 0x5c, 0xc8}},
	{64, 96, {0x7f, 0x0d, 0x9b, 0x86, 0x1f, 0x5e, 0xc1, 0xa5, 0xab, 0xf1, 0xc6, 0xa1}, {0x04, 0x6e, 0x19, 0x56, 0xb5, 0x71, 0x37, 0x93}, {0x70, 0x5b, 0xb5, 0x8e, 0x62, 0xa4, 0x7e, 0x2e}},
	{64, 96, {0x32, 0x97, 0x09, 0xf8, 0x8f, 0x1c, 0xf3, 0x0f, 0x30, 0x0f, 0x0f, 0x05}, {0xb3, 0x7c, 0xde, 0x56, 0x73, 0x50, 0x66, 0x0c}, {0x6c, 0xdb, 0x02, 0x90, 0x3c, 0x19, 0x74, 0x54}},
	{64, 96, {0x9f, 0x46, 0x8f, 0x05, 0xac, 0xa0, 0xd5, 0xaf, 0x33, 0x6f, 0xb6, 0xc9}, {0x4b, 0x32, 0x99, 0x18, 0x9c, 0x35, 0x9a, 0x53}, {0xe9, 0xdf, 0xd0, 0x17, 0xdc, 0x59, 0x4b, 0xd3}},
	{64, 96, {0x40, 0x39, 0x36, 0x9b, 0x74, 0x56, 0x08, 0x56, 0x34, 0x01, 0x39, 0xbb}, {0x06, 0x76, 0x1e, 0xdb, 0xd1, 0x45, 0x74, 0x4f}, {0x4c, 0xc5, 0x49, 0xbf, 0xcb, 0x9d, 0xa0, 0xd8}},
	{64, 96, {0x78, 0xe5, 0x39, 0x9e, 0xec, 0xdf, 0x83, 0x61, 0xd7, 0x32, 0x30, 0x61}, {0x8f, 0x8f, 0x90, 0xa5, 0xe9, 0x2e, 0x9e, 0xe7}, {0x67, 0x2d, 0xbf, 0x39, 0xea, 0x4b, 0xa4, 0xcc}},
	{64, 96, {0xe1, 0x14, 0x8f, 0xda, 0x41, 0x6e, 0x76, 0x70, 0x70, 0x62, 0xdd, 0x4d}, {0x85, 0x57, 0xb2, 0x01, 0x90, 0x20, 0x16, 0xe1}, {0xc4, 0x4f, 0xec, 0x39, 0x4c, 0x42, 0x0c, 0x6a}},
	{64, 96, {0x40, 0xe6, 0x38, 0xfa, 0xf7, 0xd6, 0x34, 0x91, 0x77, 0x56, 0xfc, 0x36}, {0x52, 0xce, 0x79, 0xbf, 0x68, 0xb3, 0xcf, 0xb1}, {0x1d, 0x47, 0xc5, 0xd2, 0x19, 0xc0, 0xf0, 0x6b}},
	{64, 96, {0x2f, 0xec, 0x62, 0x43, 0x8d, 0x52, 0x5e, 0xc7, 0xc5, 0xc4, 0x08, 0x47}, {0xf8, 0xbd, 0x69, 0xfb, 0x8c, 0x34, 0x50, 0x0f}, {0x46, 0x44, 0xad, 0x92, 0x1d, 0xd4, 0x91, 0x27}},
	{64, 96, {0xae, 0x46, 0x12, 0xb7, 0x5c, 0x62, 0x60, 0x5e, 0xab, 0xe2, 0xae, 0xd4}, {0x8d, 0xc7, 0x8d, 0xb0, 0xd4, 0xdf, 0x53, 0xb4}, {0x43, 0x6e, 0xd6, 0x3c, 0xb7, 0x2c, 0xb9, 0x89}},
	{64, 96, {0xda, 0xa6, 0x93, 0x9c, 0x36, 0x5f, 0xe9, 0x66, 0xe3, 0x79, 0xae, 0x2b}, {0x2a, 0xb3, 0xa3, 0xdb, 0x69, 0x50, 0x0d, 0xc7}, {0xb3, 0xc7, 0xe8, 0x98, 0x91, 0xd3, 0x0e, 0x06}},
	{64, 128, {0x17, 0x0a, 0x64, 0x61, 0xfb, 0xaf, 0x3d, 0x66, 0xba, 0xec, 0xf2, 0x6c, 0x7b, 0x60, 0xff, 0xb8}, {0xe9, 0xe1, 0xf1, 0x0f, 0x62, 0x97, 0xa0, 0x5a}, {0x42, 0xed, 0x38, 0x65, 0x5b, 0xd2, 0x9f, 0x13}},
	{64, 128, {0x87, 0xf4, 0x0f, 0x40, 0x23, 0xe0, 0xd5, 0xd5, 0x7c, 0x03, 0x94, 0x1b, 0xd1, 0x6e, 0xcc, 0x27}, {0x0d, 0xaa, 0x11, 0x33, 0x6b, 0xca, 0x92, 0xb4}, {0x2d, 0x23, 0xb8, 0x29, 0x61, 0x27, 0xa3, 0x66}},
	{64, 128, {0xc5, 0x71, 0xc3, 0xf2, 0x5c, 0xf4, 0xb6, 0x13, 0x56, 0xc5, 0xef, 0xf9, 0xc4, 0xbb, 0x4c, 0x3a}, {0x20, 0x14, 0x52, 0x22, 0xc5, 0x03, 0x65, 0xd6}, {0x77, 0xcf, 0xb8, 0xa2, 0xbf, 0x22, 0x8b, 0x49}},
	{64, 128, {0x2e, 0x12, 0xbd, 0xf7, 0xb3, 0x26, 0xde, 0x06, 0x4f, 0x0f, 0xae, 0xe2, 0x3a, 0xe5, 0x2d, 0xde}, {0x3a, 0xb7, 0x82, 0x98, 0xe4, 0xbb, 0x6c, 0x8f}, {0x0d, 0x7b, 0x9b, 0x9e, 0x3f, 0xc7, 0xff, 0x3e}},
	{64, 128, {0xf4, 0x44, 0x0f, 0xbc, 0x2f, 0xd8, 0x63, 0x4d, 0x35, 0x26, 0xe4, 0x81, 0xf7, 0x1b, 0x0b, 0x29}, {0xcd, 0xec, 0x93, 0x76, 0xc2, 0x25, 0x63, 0x8c}, {0xd3, 0xda, 0x4d, 0x17, 0x0c, 0x52, 0x50, 0x7e}},
	{64, 128, {0x1b, 0xaa, 0xff, 0xdf, 0xb9, 0x2b, 0xb3, 0x51, 0xd2, 0x56, 0x47, 0xd0, 0xa4, 0x76, 0xd8, 0xb0}, {0x07, 0x8a, 0x41, 0x1f, 0xbf, 0xe1, 0x6f, 0xac}, {0x65, 0x1e, 0x66, 0xc8, 0xa4, 0x0a, 0xf3, 0x45}},
	{64, 128, {0x31, 0x8e, 0xa8, 0xb8, 0x3c, 0x1f, 0x53, 0x0e, 0x3c, 0xd5, 0x49, 0xd9, 0x14, 0xbb, 0x67, 0xb3}, {0xbc, 0xad, 0x4f, 0x1f, 0x7b, 0xbd, 0x3d, 0x68}, {0x49, 0xa2, 0x76, 0xd4, 0xf7, 0x80, 0xa5, 0xc2}},
	{64, 128, {0x7f, 0x41, 0x4e, 0xa4, 0x27, 0x54, 0xab, 0xa0, 0x05, 0xc0, 0xb2, 0xe2, 0x90, 0xe7, 0x2f, 0xfa}, {0x92, 0xec, 0xe4, 0x0f, 0x21, 0xaf, 0xfb, 0x99}, {0xd5, 0x8f, 0xc7, 0x59, 0x90, 0x49, 0x18, 0xd1}},
	{64, 128, {0x64, 0x4e, 0x7b, 0x2b, 0x6b, 0x4c, 0xff, 0xda, 0x97, 0x7a, 0xfb, 0xb7, 0xd7, 0x59, 0x0c, 0x87}, {0x32, 0x8a, 0x31, 0x79, 0xef, 0xb7, 0x27, 0xba}, {0x30, 0x2a, 0x80, 0xc1, 0xf2, 0x0f, 0x73, 0xee}},
	{64, 128, {0x36, 0x68, 0x89, 0x70, 0x84, 0xdb, 0x9e, 0x6f, 0x2d, 0xfe, 0x66, 0x42, 0x6a, 0x17, 0x42, 0xa8}, {0x8a, 0xf7, 0x2d, 0xfd, 0x94, 0x3e, 0x14, 0x48}, {0x1a, 0x6b, 0x3b, 0x14, 0x3c, 0xaf, 0xf0, 0x15}},
	{96, 96, {0x62, 0xf4, 0x02, 0x60, 0x99, 0x65, 0x65, 0x3d, 0x4a, 0xef, 0x3d, 0x1e}, {0xa0, 0xbe, 0x4a, 0x44, 0xb7, 0x56, 0xe4, 0x48, 0x7f, 0xef, 0x5b, 0xc2}, {0xa1, 0x46, 0x4a, 0x6b, 0xa7, 0xb4, 0x37, 0x70, 0x6a, 0xe4, 0x39, 0x03}},
	{96, 96, {0x79, 0x39, 0x94, 0xea, 0x4d, 0x6d, 0x3f, 0x06, 0x8b, 0x51, 0x09, 0x12}, {0x15, 0xe1, 0xa9, 0x46, 0x51, 0x42, 0xbe, 0xac, 0x7c, 0x31, 0xa9, 0x16}, {0xa9, 0xff, 0x1a, 0x65, 0x63, 0xdd, 0x94, 0xa8, 0x50, 0x65, 0x85, 0x2e}},
	{96, 96, {0xcc, 0x08, 0xdf, 0xd8, 0x18, 0x35, 0x0f, 0x9e, 0x24, 0x99, 0x93, 0x43}, {0xdb, 0x36, 0xcb, 0x01, 0x73, 0x5b, 0x77, 0x14, 0x36, 0xed, 0xd4, 0xf5}, {0xa4, 0xbf, 0x54, 0x82, 0xcd, 0x4e, 0xd6, 0xaf, 0x09, 0x6e, 0x76, 0xe7}},
	{96, 96, {0x40, 0x4d, 0x50, 0xe7, 0x3e, 0xf5, 0x4f, 0x30, 0x04, 0x7d, 0x8d, 0x35}, {0x4e, 0xb9, 0x27, 0x7d, 0xb6, 0x22, 0xfc, 0xa2, 0xc5, 0xbd, 0x42, 0x49}, {0xf7, 0x8b, 0xb3, 0xc6, 0x01, 0x15, 0xf8, 0xe1, 0x53, 0xcf, 0x89, 0xd3}},
	{96, 96, {0x6c, 0xdb, 0x83, 0xfb, 0x71, 0x26, 0x12, 0x19, 0xcc, 0x33, 0x1d, 0x74}, {0x46, 0x52, 0xe7, 0x48, 0x35, 0x60, 0x0b, 0x09, 0x71, 0x7b, 0x68, 0x47}, {0x83, 0xeb, 0xb0, 0xb1, 0xa2, 0xc1, 0x61, 0x03, 0x7f, 0x83, 0xed, 0x45}},
	{96, 96, {0x73, 0x5a, 0xb4, 0x1f, 0xea, 0xef, 0x77, 0xfb, 0x90, 0xb2, 0xe1, 0x56}, {0xde, 0x23, 0x0f, 0x7c, 0xca, 0xb3, 0x16, 0x79, 0x32, 0x46, 0xdf, 0x95}, {0x95, 0x65, 0x5d, 0x6d, 0x17, 0xfe, 0x19, 0x08, 0xd0, 0x87, 0x9e, 0x93}},
	{96, 96, {0xd3, 0xc4, 0x86, 0x2e, 0xf4, 0xf3, 0x70, 0x47, 0xfa, 0xbc, 0x70, 0x6b}, {0x81, 0x2a, 0x09, 0x84, 0x9e, 0x47, 0x48, 0x7e, 0xea, 0x8e, 0xc1, 0xa3}, {0xd1, 0x22, 0x2d, 0x15, 0xf2, 0x91, 0xa1, 0xc3, 0x99, 0xb9, 0x94, 0x08}},
	{96, 96, {0x38, 0xfa, 0x9e, 0x23, 0x9f, 0x5c, 0xc3, 0x97, 0x7c, 0xf1, 0x0c, 0xe0}, {0x5a, 0x48, 0xc2, 0x7e, 0xf4, 0xad, 0xc4, 0xde, 0xd8, 0x0b, 0xea, 0xc6}, {0x8e, 0x40, 0xa7, 0x27, 0x54, 0x69, 0xee, 0x9c, 0xd6, 0x17, 0x4f, 0x3c}},
	{96, 96, {0xa5, 0x8b, 0x2d, 0xe9, 0x4d, 0x27, 0xfa, 0x8a, 0xef, 0xd0, 0xfb, 0x71}, {0xe9, 0xd1, 0xb1, 0xb5, 0x5e, 0x94, 0x57, 0xc4, 0x58, 0xc0, 0xba, 0xf7}, {0x57, 0xde, 0x82, 0x92, 0xca, 0x90, 0xee, 0x10, 0xc3, 0xd8, 0x8d, 0xf6}},
	{96, 96, {0xb1, 0x89, 0xca, 0x31, 0xfd, 0xb1, 0x2e, 0x5d, 0x13, 0xff, 0xd5, 0xc1}, {0xa7, 0x28, 0x38, 0x5c, 0xe5, 0x4e, 0x46, 0x90, 0x54, 0x59, 0xd9, 0xaa}, {0xb2, 0xd6, 0x47, 0xf7, 0x5a, 0xd3, 0x94, 0x99, 0x4c, 0xb2, 0x0d, 0xc5}},
	{96, 144, {0xd5, 0x6d, 0xcd, 0x07, 0x40, 0xc7, 0x1f, 0x33, 0x23, 0x39, 0x63, 0x6f, 0x2d, 0x16, 0x5a, 0x54, 0xf2, 0x2c}, {0xc9, 0xab, 0xfc, 0xe8, 0x35, 0x41, 0x06, 0x8d, 0x97, 0x27, 0x07, 0x07}, {0x9b, 0x67, 0x4e, 0x6f, 0x5d, 0x87, 0xdf, 0x06, 0x3c, 0x61, 0xea, 0x36}},
	{96, 144, {0xb4, 0x60, 0x94, 0x22, 0x44, 0xa2, 0x75, 0x5a, 0x87, 0xc9, 0x3f, 0x4f, 0xb7, 0x22, 0x99, 0x36, 0x66, 0xde}, {0xe1, 0x68, 0x08, 0x3b, 0x83, 0x52, 0xa6, 0x3f, 0x64, 0x0e, 0x0e, 0xc1}, {0x76, 0xc8, 0x43, 0x44, 0x0e, 0x88, 0xc4, 0x04, 0x35, 0x5e, 0x11, 0x81}},
	{96, 144, {0xc7, 0xb9, 0x64, 0x19, 0x0d, 0x47, 0xc7, 0x2a, 0x63, 0x97, 0x6d, 0x56, 0x90, 0xb8, 0xf2, 0xc3, 0xfe, 0xb4}, {0xf4, 0x5f, 0xdc, 0xcc, 0x0e, 0xd8, 0x76, 0xb8, 0x0a, 0x64, 0x84, 0xab}, {0x80, 0x4f, 0x21, 0xcf, 0x65, 0x86, 0xf5, 0xfc, 0x8c, 0xdf, 0xf4, 0xd1}},
	{96, 144, {0x80, 0x20, 0xfc, 0xd9, 0x2e, 0x0a, 0x4e, 0xa8, 0x19, 0x96, 0xeb, 0xab, 0xd1, 0x9e, 0x21, 0x1d, 0x13, 0x0d}, {0xcb, 0xb5, 0x71, 0xf6, 0x0c, 0x7e, 0x0b, 0x53, 0xe5, 0x56, 0xb7, 0x23}, {0x5e, 0xfd, 0x3f, 0xa2, 0xb0, 0xca, 0x24, 0x56, 0xce, 0xeb, 0x1c, 0x2f}},
	{96, 144, {0xcd, 0xdd, 0xda, 0xaf, 0x96, 0x8b, 0x86, 0xa1, 0x1f, 0x82, 0x15, 0x3f, 0x6c, 0x5d, 0x45, 0x10, 0x56, 0xfa}, {0xc5, 0x3f, 0x7b, 0x5a, 0xd5, 0x77, 0x93, 0xb7, 0x79, 0xb6, 0x1e, 0x3a}, {0xc5, 0x6c, 0x6a, 0x55, 0x52, 0x38, 0x86, 0xa7, 0xb3, 0xe3, 0xa0, 0x88}},
	{96, 144, {0x06, 0xe4, 0x4f, 0x7f, 0x6f, 0x29, 0x5d, 0xa7, 0x91, 0xe6, 0x90, 0x20, 0x5b, 0xd1, 0xcc, 0xfb, 0x39, 0x8a}, {0xae, 0x4c, 0x7d, 0xef, 0x97, 0xc0, 0x28, 0x0d, 0x19, 0x6f, 0xbd, 0xda}, {0xd0, 0x3d, 0xdb, 0x51, 0x39, 0xc1, 0x49, 0x44, 0xbf, 0x38, 0x9d, 0x72}},
	{96, 144, {0x0f, 0xb7, 0x34, 0xec, 0x04, 0x40, 0xaf, 0x9a, 0xc2, 0x15, 0x40, 0xfb, 0x52, 0x14, 0xbd, 0x3c, 0x5e, 0xd8}, {0x67, 0xa0, 0x5b, 0xc2, 0xa2, 0xd2, 0x45, 0xae, 0x45, 0xcc, 0x60, 0xf1}, {0xb1, 0xb7, 0x1f, 0xee, 0x93, 0xb8, 0xc0, 0xef, 0x70, 0xf8, 0x31, 0xe8}},
	{96, 144, {0x02, 0x90, 0x0b, 0x29, 0xe2, 0x35, 0x9b, 0x0b, 0x2c, 0xe1, 0xe8, 0x09, 0x34, 0xee, 0x05, 0xbb, 0x9d, 0x99}, {0x3c, 0x94, 0x9e, 0xc3, 0xe1, 0x2d, 0xbf, 0xc9, 0x5b, 0xd2, 0xd4, 0x76}, {0xa6, 0xf2, 0xd8, 0x56, 0x13, 0x7e, 0x91, 0x8b, 0x67, 0xbe, 0x43, 0xcd}},
	{96, 144, {0xee, 0x96, 0x5f, 0x4b, 0xd6, 0x57, 0x47, 0x70, 0x20, 0x1a, 0xa9, 0x8e, 0x09, 0x5f, 0xbe, 0x1b, 0x58, 0x8b}, {0x43, 0x81, 0x3f, 0x1a, 0xf4, 0x58, 0xff, 0x9c, 0x91, 0x5f, 0x31, 0x24}, {0x8b, 0xb5, 0xa6, 0x93, 0xbe, 0x8d, 0x98, 0xa6, 0x08, 0xd4, 0xb6, 0x9e}},
	{96, 144, {0x36, 0x30, 0xd5, 0xb8, 0x24, 0xb7, 0xbd, 0x05, 0x31, 0xa6, 0xf5, 0xef, 0x98, 0x15, 0x51, 0x5b, 0x6a, 0x87}, {0xb1, 0xcf, 0x02, 0xdb, 0x5d, 0x5a, 0xa7, 0xda, 0x7d, 0x42, 0x83, 0x6b}, {0xc0, 0x46, 0x1e, 0x10, 0xb1, 0x27, 0xa3, 0xf9, 0xda, 0xd7, 0xfa, 0x25}},
	{128, 128, {0x53, 0x43, 0x9c, 0x46, 0x19, 0xb2, 0xd2, 0x1a, 0xcf, 0x8b, 0x37, 0xe9, 0x80, 0x49, 0xa8, 0xc8}, {0xfa, 0x2a, 0xc2, 0x32, 0xac, 0xd1, 0x3e, 0x49, 0xb5, 0x64, 0xf5, 0xa5, 0x4b, 0x6e, 0x36, 0x7d}, {0x0d, 0xff, 0x25, 0x33, 0x5f, 0x01, 0x2e, 0x2f, 0x82, 0x99, 0xc3, 0xef, 0xf2, 0xf6, 0x96, 0x84}},
	{128, 128, {0xf0, 0x20, 0x21, 0x3f, 0xdf, 0x79, 0xcd, 0xcc, 0x59, 0x9a, 0x3a, 0xbf, 0xfb, 0x61, 0xdf, 0x32}, {0xb2, 0x88, 0x12, 0x7f, 0xe1, 0x6d, 0x88, 0x6f, 0x80, 0x33, 0x61, 0xd6, 0x78, 0x6c, 0xe4, 0x2a}, {0x99, 0x67, 0x9c, 0xbb, 0xe1, 0xbe, 0x9e, 0x4e, 0x6c, 0x64, 0xb1, 0x78, 0x66, 0x79, 0xc5, 0x08}},
	{128, 128, {0xdd, 0x95, 0x72, 0xfe, 0x95, 0xa3, 0x5f, 0xec, 0xd9, 0x9f, 0x48, 0x87, 0x4c, 0x78, 0xec, 0x4b}, {0x4c, 0x6c, 0x5d, 0xbf, 0x08, 0xf9, 0x11, 0xdd, 0xff, 0x5f, 0x8e, 0x05, 0x2d, 0x37, 0xfb, 0x68}, {0x73, 0x44, 0xfe, 0x3e, 0x41, 0x57, 0x69, 0x6e, 0x73, 0x35, 0x70, 0x75, 0x2d, 0xd7, 0xf3, 0x84}},
	{128, 128, {0x57, 0xe0, 0x50, 0xe2, 0xf3, 0xfa, 0xc0, 0xa7, 0xb6, 0xaf, 0x8a, 0x70, 0x84, 0x4c, 0xac, 0xc8}, {0x90, 0xb6, 0xd5, 0x12, 0x4f, 0x11, 0xde, 0xed, 0x9a, 0x2a, 0x21, 0x79, 0x18, 0x24, 0xa4, 0xb7}, {0x2b, 0x7a, 0x7b, 0x28, 0x63, 0xd8, 0x9b, 0x20, 0x48, 0x2c, 0x37, 0x7d, 0x14, 0xe4, 0xfe, 0x7a}},
	{128, 128, {0xcd, 0x08, 0x42, 0xed, 0x7c, 0xdf, 0x1d, 0x66, 0xa2, 0x65, 0x3b, 0x9f, 0xf0, 0x53, 0x52, 0x6e}, {0xf0, 0x78, 0xbf, 0xbf, 0xc2, 0x07, 0x90, 0xec, 0xa6, 0xf0, 0x1d, 0x22, 0x4a, 0x8f, 0xd8, 0x8f}, {0x1c, 0xa7, 0xd7, 0x42, 0xd3, 0x4f, 0x8d, 0x05, 0x00, 0x1b, 0x0d, 0xca, 0x7b, 0x06, 0xce, 0x3c}},
	{128, 128, {0x65, 0x20, 0x8d, 0x69, 0xf2, 0x7a, 0x54, 0x51, 0xca, 0x24, 0x69, 0xc7, 0x23, 0xcf, 0x50, 0xc4}, {0xd2, 0xf2, 0xcd, 0xd2, 0xc6, 0x66, 0x44, 0x93, 0xfd, 0x52, 0xd6, 0xb0, 0xee, 0x58, 0xad, 0xfe}, {0x52, 0xd0, 0x82, 0xd7, 0x87, 0x78, 0x66, 0xb4, 0x61, 0x7d, 0x2d, 0xe7, 0x4a, 0x27, 0xec, 0xf9}},
	{128, 128, {0x4f, 0xd6, 0xc1, 0x72, 0x0c, 0x16, 0xcb, 0x7c, 0x56, 0xcc, 0x77, 0x14, 0xe5, 0x43, 0xce, 0x39}, {0x32, 0xb4, 0x66, 0x3a, 0xe2, 0x16, 0x4e, 0x4e, 0x01, 0xaf, 0xdb, 0xd2, 0x86, 0x74, 0x63, 0x2e}, {0x1f, 0x89, 0xaa, 0xbf, 0xe9, 0xe5, 0x45, 0xce, 0xae, 0xc5, 0xa5, 0x85, 0x03, 0x6b, 0xb8, 0x46}},
	{128, 128, {0x5c, 0x54, 0x75, 0x3d, 0xd7, 0x9c, 0x6b, 0x56, 0x59, 0xc7, 0x71, 0xd1, 0xe9, 0x69, 0x4c, 0xb9}, {0xf9, 0x2d, 0x38, 0x74, 0x81, 0x48, 0xf5, 0x77, 0x33, 0x7e, 0x49, 0x01, 0xe0, 0x2e, 0x53, 0xf4}, {0xd0, 0x8b, 0x47, 0x48, 0xdc, 0x16, 0x3c, 0x74, 0x53, 0xcd, 0x26, 0x4f, 0x5f, 0xee, 0xdb, 0x90}},
	{128, 128, {0x27, 0xea, 0x33, 0xbc, 0x56, 0x93, 0xcf, 0x06, 0xa9, 0x66, 0x71, 0xc3, 0xa5, 0x5b, 0x5a, 0x41}, {0x39, 0xd3, 0x43, 0x80, 0x8e, 0x85, 0x7d, 0x91, 0x83, 0x07, 0x06, 0xdc, 0xfb, 0x76, 0xf6, 0xe4}, {0x50, 0x88, 0x48, 0xc3, 0xbb, 0x65, 0xd8, 0x9c, 0x83, 0x76, 0x5a, 0xe5, 0x0e, 0xfb, 0xd0, 0xe6}},
	{128, 128, {0x2b, 0x33, 0x40, 0x82, 0xcc, 0x75, 0xf5, 0x27, 0xb5, 0xb6, 0x48, 0xdd, 0xac, 0x5c, 0x7d, 0x34}, {0x25, 0x5f, 0x49, 0x25, 0x16, 0x61, 0x8a, 0x7f, 0xc0, 0x94, 0x83, 0x07, 0xae, 0x1b, 0xd8, 0xef}, {0x9b, 0xbc, 0x30, 0x4e, 0x93, 0xaf, 0x14, 0xe2, 0xbe, 0xf9, 0x84, 0x0b, 0x39, 0xf0, 0x53, 0x57}},
	{128, 192, {0x5f, 0xb3, 0xcc, 0x23, 0xb5, 0x27, 0xb2, 0xdd, 0x92, 0xb7, 0xb1, 0x21, 0x39, 0x99, 0x61, 0x27, 0x4e, 0x07, 0x26, 0x5f, 0x09, 0x12, 0x4b, 0x2c}, {0xca, 0x39, 0xba, 0x5a, 0x4b, 0xdc, 0xdb, 0x4b, 0xc3, 0x1e, 0xb3, 0x5e, 0x69, 0x6a, 0x93, 0xf8}, {0x9b, 0x7e, 0xdc, 0xe1, 0xad, 0xb1, 0xe0, 0x0b, 0x4c, 0xee, 0x2d, 0xf2, 0x7d, 0xcf, 0xc2, 0x8e}},
	{128, 192, {0x6b, 0x06, 0x78, 0x27, 0x3e, 0xbe, 0x9c, 0xca, 0x30, 0x10, 0x0a, 0x0d, 0x87, 0x06, 0xe2, 0x67, 0x4b, 0x74, 0xd8, 0xb7, 0xf0, 0x08, 0x00, 0xe1}, {0xc0, 0x4b, 0x9d, 0xa6, 0x5b, 0x51, 0xa0, 0x15, 0x6c, 0xf3, 0x18, 0xfa, 0x08, 0xed, 0xcb, 0x6f}, {0xe7, 0xc6, 0x2f, 0x38, 0x64, 0xce, 0xa9, 0x20, 0x8a, 0xcb, 0x8b, 0xbd, 0xe0, 0x9e, 0x02, 0x93}},
	{128, 192, {0x5d, 0x8e, 0xc3, 0xa5, 0xe7, 0x4c, 0xff, 0x57, 0xab, 0x0c, 0x35, 0xc8, 0x59, 0x77, 0x6a, 0x22, 0x5d, 0xba, 0x9e, 0xfb, 0x57, 0x71, 0x3d, 0xd2}, {0x39, 0xb5, 0x94, 0x77, 0x21, 0xcb, 0x37, 0x24, 0xc6, 0x05, 0xed, 0x95, 0x73, 0x0c, 0xa1, 0xc3}, {0xcb, 0xf3, 0x13, 0xca, 0xf0, 0xd8, 0xc2, 0xe3, 0xf6, 0xc0, 0x5a, 0x79, 0x53, 0x9a, 0x3a, 0xd0}},
	{128, 192, {0x67, 0xbf, 0x9d, 0x77, 0xb0, 0xb4, 0x21, 0x59, 0xc7, 0xb9, 0x86, 0x14, 0xfe, 0x34, 0xf6, 0x6a, 0x64, 0xda, 0xd2, 0xff, 0xbb, 0x06, 0xc9, 0x81}, {0x92, 0x54, 0xd5, 0xe6, 0xab, 0x76, 0xa3, 0xdb, 0xce, 0xb7, 0xe2, 0x1c, 0xbd, 0xe6, 0xdc, 0x92}, {0xd2, 0x2e, 0x42, 0x21, 0x1d, 0xa0, 0x37, 0xf5, 0xb8, 0xc8, 0xb0, 0xc4, 0x73, 0x98, 0x16, 0xcb}},
	{128, 192, {0x50, 0xdb, 0xb9, 0x4d, 0xfc, 0x93, 0x5d, 0x69, 0x63, 0x60, 0x58, 0x43, 0xf5, 0xa4, 0xda, 0x92, 0x51, 0x64, 0x86, 0x00, 0x85, 0x5b, 0x1a, 0xe6}, {0xec, 0x56, 0x0b, 0x4e, 0xd3, 0xe9, 0x37, 0xfb, 0xfc, 0xe4, 0xd2, 0xac, 0x3d, 0x5e, 0x50, 0x91}, {0x37, 0x8c, 0x07, 0x88, 0x33, 0x87, 0xfc, 0x6d, 0xde, 0x08, 0xa6, 0x2f, 0x29, 0x22, 0x21, 0xd2}},
	{128, 192, {0x99, 0x91, 0x5f, 0x12, 0x47, 0x6e, 0x02, 0xb3, 0x6c, 0x81, 0x42, 0x27, 0x26, 0x28, 0x5b, 0x92, 0x74, 0xdf, 0xf8, 0x24, 0x94, 0x1e, 0x4e, 0x1d}, {0xfd, 0x99, 0x63, 0x32, 0x4f, 0x35, 0x69, 0x86, 0x4d, 0x5a, 0x61, 0x9c, 0x4f, 0xc4, 0x9b, 0x13}, {0xbe, 0x08, 0x4d, 0xa2, 0x87, 0xde, 0xe8, 0xa3, 0x1a, 0x81, 0xcc, 0xad, 0xd0, 0x37, 0x2e, 0xa1}},
	{128, 192, {0x43, 0x4a, 0x14, 0xae, 0x5f, 0x3e, 0x83, 0x59, 0xbc, 0xb5, 0xc2, 0x47, 0x97, 0x1b, 0xa8, 0x11, 0xc1, 0xc7, 0x4d, 0x0e, 0x74, 0x79, 0x69, 0x7a}, {0x04, 0x74, 0x30, 0x6a, 0xdd, 0x3b, 0x80, 0x5b, 0xda, 0x49, 0x6d, 0xa7, 0xec, 0x7f, 0xb7, 0x28}, {0x50, 0x61, 0x01, 0xe7, 0x5a, 0x6e, 0x56, 0x44, 0xee, 0x18, 0xa8, 0x1d, 0x2c, 0x51, 0x7c, 0xd1}},
	{128, 192, {0x60, 0x22, 0x57, 0x0d, 0xda, 0x44, 0x73, 0x6b, 0xd7, 0x9c, 0x32, 0xed, 0xe9, 0xaa, 0xb9, 0x90, 0x21, 0xeb, 0xa2, 0xe1, 0x36, 0x74, 0x4a, 0x76}, {0x31, 0xf5, 0x1f, 0xf4, 0xba, 0x76, 0x85, 0x6b, 0x49, 0xca, 0x09, 0x79, 0xd7, 0x04, 0x35, 0xc3}, {0x37, 0xfe, 0x2a, 0x88, 0x12, 0x19, 0x73, 0x55, 0x84, 0x36, 0x1b, 0x00, 0x84, 0xe0, 0x39, 0xf6}},
	{128, 192, {0xb3, 0xc5, 0x62, 0xd8, 0x81, 0xc9, 0x5b, 0x0a, 0x03, 0x60, 0xaf, 0xa1, 0x0d, 0x85, 0x90, 0x43, 0x8d, 0xeb, 0xb0, 0xd8, 0x54, 0x61, 0xf8, 0x90}, {0x22, 0x7d, 0x4e, 0x51, 0x07, 0x65, 0x39, 0x50, 0x47, 0xa7, 0x3f, 0x4e, 0xfd, 0x8e, 0x5c, 0x96}, {0xf2, 0x84, 0x81, 0x28, 0x14, 0x99, 0x7b, 0x43, 0xfd, 0x6b, 0x63, 0xbe, 0xff, 0xbe, 0x1f, 0x1d}},
	{128, 192, {0xd0, 0xc8, 0x23, 0x27, 0x3f, 0x83, 0xfd, 0xbc, 0xaa, 0xb2, 0xab, 0x55, 0xc9, 0xe1, 0x34, 0x9d, 0x77, 0x6a, 0x19, 0xde, 0x20, 0xfe, 0x5d, 0xf8}, {0xc2, 0x32, 0x8e, 0xe8, 0xcc, 0xfb, 0xf3, 0x38, 0xab, 0x39, 0x82, 0x4c, 0x3f, 0x11, 0x2c, 0x1b}, {0x8c, 0xfb, 0xcd, 0x63, 0xd5, 0x40, 0xd1, 0xa8, 0x09, 0x19, 0x1b, 0x24, 0x3b, 0x3b, 0xee, 0xa5}},
};