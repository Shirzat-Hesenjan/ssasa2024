// Borrow from https://github.com/euugenechou/sha3-256 to do basic IO for the programme and print test results
// Note that the reference code contains full NIST test vectors, there's no need to run the reference implementation's result,
// instead if my implementation pass the tests, it should suffice for correctness.

#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* this is my implementation*/
#include "sha3.h"

// bool verbose = false;

// void output(char* fmt, ...) {
//   if (verbose) {
//     va_list arg;
//     va_start(arg, fmt);
//     vfprintf(stderr, fmt, arg);
//     va_end(arg);
//   }
// }

// void check(bool cond, char* fmt, ...) {
//   if (!cond) {
//     va_list arg;
//     va_start(arg, fmt);
//     vfprintf(stderr, fmt, arg);
//     va_end(arg);
//     exit(EXIT_FAILURE);
//   }
// }

// void hexprint(uint64_t nbytes, uint8_t bytes[static nbytes]) {
//   for (uint64_t i = 0; i < nbytes; i += 1) {
//     output("%02" PRIx8, bytes[i]);
//   }
//   output("\n");
// }

// /*This is the method to call the reference implementation using `hashfile`*/
// void hash_file(int64_t length, FILE* infile, FILE* outfile) {
//   unsigned char* msg =
//       (unsigned char*)calloc(length ? length : 1, sizeof(unsigned char));
//   check(msg, "Failed to allocate array for message.\n");

//   output("Len = %" PRId64 "\n", length);
//   fread(msg, sizeof(uint8_t), length, infile);

//   output("Msg = ");
//   hexprint(length, msg);

//   unsigned char* md = keccak_hash(msg, length);

//   output("MD = ");
//   hexprint(SHA3_256_DIGEST_SIZE, md);

//   fwrite(md, sizeof(uint8_t), SHA3_256_DIGEST_SIZE, outfile);
//   free(msg);
// }

// void print_usage(char** argv) {
//   fprintf(
//       stderr,
//       "SYNOPSIS\n"
//       "   Produce hash-codes for input bytes using the SHA-3 256 algorithm.\n"
//       "\n"
//       "USAGE\n"
//       "   %s [-hv] -l length [-i input] [-o output]\n"
//       "\n"
//       "OPTIONS\n"
//       "   -h          Display program help and usage\n"
//       "   -v          Display verbose program output\n"
//       "   -l length   Length of input in bytes\n"
//       "   -i input    Specify input to hash (stdin by default)\n"
//       "   -o output   Specify output of hashed input (stdout by default)\n",
//       argv[0]);
// }

// #define OPTIONS "hvl:i:o"

// int main(int argc, char** argv) {
//   int opt = 0;
//   int64_t length = -1;
//   FILE* infile = stdin;
//   FILE* outfile = stdout;

//   while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
//     switch (opt) {
//       case 'h':
//         print_usage(argv);
//         return 0;
//       case 'v':
//         verbose = true;
//         break;
//       case 'l':
//         length = (int64_t)strtoll(optarg, NULL, 10);
//         break;
//       case 'i':
//         infile = fopen(optarg, "rb");
//         check(infile, "Failed to open %s.\n", optarg);
//         break;
//       case 'o':
//         outfile = fopen(optarg, "wb");
//         check(outfile, "Failed to open %s.\n", optarg);
//         break;
//       default:
//         print_usage(argv);
//         return 1;
//     }
//   }

//   check(length >= 0, "Valid message length must be supplied.\n");

//   hash_file(length, infile, outfile);

//   fclose(infile);
//   fclose(outfile);
//   return 0;
// }

// some utilities
void print_digest(unsigned char* digest) {
  int i;
  for (i = 0; i < SHA3_256_DIGEST_SIZE; i++) {
    printf("%02x", digest[i]);
  }
  printf("\n");
}

uint64_t expected[25][25] = {
    /* after round 1 */
    {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
    /* after round 2 */
    {0x8083,
     0x100000000000,
     0x8000,
     0x1,
     0x100000008000,
     0x0,
     0x200000200000,
     0x0,
     0x200000000000,
     0x200000,
     0x2,
     0x200,
     0x0,
     0x202,
     0x0,
     0x10000400,
     0x0,
     0x400,
     0x10000000,
     0x0,
     0x10000000000,
     0x0,
     0x10000000004,
     0x0,
     0x4},
    /* after round 3 */
    {0x8030500001e0840c, 0x38ca983082300106, 0x8b0340041c14101,
     0x2002783081a00483, 0x10488c0040d14100, 0x49840d3042220,
     0x11932210b0700202, 0x40038e20070020f0, 0x31953040d0600010,
     0x60030c30241400c2, 0xa50434031950,     0x3e0038090a8080,
     0xc400828e0c100043, 0x22263021081812,   0xc41c018a18108081,
     0x1840c580e382590,  0x3e08505001060000, 0x40c0061063a6584,
     0x39801c4809030010, 0xa08402100074184,  0x80859c8008192199,
     0xc700080080e020e2, 0x8584804017321c,   0x4100180008e801a1,
     0x6000c00c0063266},
    /* after round 4 */
    {0x838573a4deb6243,  0xcf57a0c778692494, 0x558a7a103de1a9f7,
     0x7ebf8bcaa7384087, 0xc20e7c2f247f1fc3, 0x8b4b752fe1ff3dd4,
     0xcaf94e2ea308a3de, 0x84157265ed62c980, 0x95eb44cdb62a1f93,
     0x10401cdb131f356a, 0x43149ae4f34cae4e, 0xc7ff1e37f2a46df1,
     0x9082bcd5a1256e14, 0x57c528940923d229, 0xd68102405f743186,
     0xdd83725682ce30ad, 0xa99560e9a64817a1, 0xbab5ebd7d1623c17,
     0x9c1ed728fdc103ad, 0xe8a581be707cb952, 0x81bc65422742b355,
     0x9c66d82b94503aa2, 0x4f75a9de463907d4, 0xe90eb8a4080a858f,
     0x6542bc007f6bd7a3},
    /* after round 5 */
    {0x6a00840802752a6f, 0xf9a9c3ab00c9c931, 0x6db98725571f1604,
     0x96ba275ba7474a93, 0x9b5e7a3feeb7e41e, 0x73fe33c9b1038c36,
     0x70e8b5d763274728, 0xfe03a842f22afcb,  0xd95c0a4ec94ad619,
     0xe57a1a2bb2ae09c2, 0x2728e4f2b7aeebe8, 0xa81edbeb54d20fe1,
     0x7ac22599684b0182, 0xc17e6a6ab6526eb1, 0xa1c3cee067ab9f52,
     0xb8e36f84d019b15f, 0x1cf47f1f04738ad9, 0xf377844620f2d499,
     0x105a64a116516b0e, 0x965393ca1b42f1db, 0x1c2c849d3fd29c1f,
     0xab89b3623f5f6964, 0x916d0713d86acc2a, 0xe5dc85ffad78d9b0,
     0x650759748df5efe9},
    /* after round 6 */
    {0x43e30b96ff110a58, 0xd642c7df22b4173c, 0xbfd660dfe2e0051d,
     0xa303b734f55677e6, 0x37c05e405b01af0c, 0xb5033314f45c5cb,
     0x44f0fbf5606f647a, 0x34fe5a6214181ed1, 0xb42abc5de738ddd5,
     0x7a8e099fe258e5d8, 0x2d6cbeb27c4bc219, 0xa58159b967a3ad93,
     0xa4036d7eaf457157, 0x5f252e8e367f3dc8, 0x5e8f6ad5d330526e,
     0x4fcb128386812f6e, 0x2c154103bd90dd0,  0x375463e874af271c,
     0x1cee752edb4b48f6, 0x36e67b423707dcb0, 0xf36ba8b04378f57a,
     0xcfcd9cecd3ade3d5, 0x7fc73c7b61494b4c, 0xb913d9d348a8e89a,
     0x1edca3008e4023a2},
    /* after round 7 */
    {0xc77f40134a70cbc,  0x6f650cbffc57ab01, 0x6964c6213cd3202d,
     0xf5841db03d841837, 0xa7158bb1a7922376, 0x42b502eff3fce7df,
     0xa42f64e793328d4f, 0x5640794d9b8e0201, 0x2acf091a0c121018,
     0xe7c314743c763a96, 0x179b677365470e31, 0x28bb4a3ea45041a4,
     0x8bacdac06316344c, 0x6989c2111a451228, 0x14bc213555dcb238,
     0xaf39326eb3d059f6, 0x213d3c0c7d694b60, 0x3874ebc3cf5c4aef,
     0xd809ca879d35da1c, 0x41ad501b5eaa2c21, 0x2727f7e1d04b5609,
     0x4f40bfbf5114b70e, 0x87ed62a9c0e907db, 0xefd2f940cf583917,
     0x675ac1b705f06e75},
    /* after round 8 */
    {0xe71990f66e5c3199, 0x9815a7fefc970812, 0x7adeb6308d077a89,
     0x1bcf81ec65528647, 0xfb3f99d0c00b9d77, 0x9debe90720187a5e,
     0x90d8ccbb6e0c2505, 0x71fa1d117d2f0d2b, 0x7e2d04cc17121756,
     0xc31e4540d69f88c9, 0xd0316755555ceec9, 0x5dcd9aa3fcdad534,
     0x99b4d176e93e4c6c, 0x2a1c42a54f48b263, 0x439eb6c088b33bb8,
     0x3de51a3100441002, 0x92fd96a29fb9ca,   0x641e17838270a7af,
     0xc09f6d8491b94fe3, 0xdb137cc98c3c8239, 0xad3beec10f39b07f,
     0x6b62b259d8ff954c, 0x965f3f9b90b4ba79, 0x5e28c4ce78bd2190,
     0x22d0d89cad79a8ae},
    /* after round 9 */
    {0x309632a97ced9ce8, 0x96cce37672278288, 0xb8b4102d555463a0,
     0x781cb76c1b390af6, 0x5efe9079fbd06374, 0x8c004295c6f66cea,
     0x7063b2fd56e7e2e5, 0x5c202896127a3201, 0xde117f77f405d44b,
     0x93350e8d9591bc9e, 0x4735c6634ea08aa2, 0x479dabb00fd1751d,
     0xd06cfd3ab48c6998, 0xe4c33c7972e95e5c, 0x525049f077812bc0,
     0x31a6e97ccae2238d, 0x43976056b7e1952d, 0x4a71a7f66478dd99,
     0x2adef1f0f476bc2f, 0xe9231008af083f0c, 0xf19d1873be49dca9,
     0xf7cc92e1c5618e18, 0x3aba6fb87aad63aa, 0x73f3b6a47154402f,
     0x4577f0cb2ccb722a},
    /* after round 10 */
    {0x31cbb7f055d0efc0, 0xe4b25c70ddc208d1, 0x9da2f29054ce16ee,
     0x7e8db0b9132d1f74, 0xb563450ecb53cae8, 0xc639e6d4dbb1c52,
     0x33386e70b4edfae2, 0xd14705c13c413b26, 0x8fb3a1c9623230f1,
     0x928ac03bb01eb0a4, 0x1a235d324d3e8a10, 0x23b9113dc331409e,
     0x79c57354bd39debc, 0xf068bde5cf48f1b5, 0x5b22dd2d64fec38f,
     0x759959b043b15e3e, 0x9d7e94ca55db4480, 0x968da580db523741,
     0xa68326239c1e7d24, 0xb534d89df4ee2434, 0x65869f220f62e42e,
     0x28fedc4ead93fed0, 0xbb6d3c3549da2b97, 0x61089150769da6cf,
     0x18b378b6eeec04b1},
    /* after round 11 */
    {0x736d2c1d55c4ef73, 0x2e734b02ce704378, 0xbfaf17a701e92e36,
     0x5c9b10c4952fd875, 0xcdc8e10f6ba13c50, 0xf5f58fa9fc4545d6,
     0xf17d3c1327115fef, 0xe4af9f9f755c1079, 0x5a19d348ebbbb880,
     0xa4e4fc5a17f63732, 0xe14da812846248af, 0xd3376cb3ecec921b,
     0x660e9235b4dc7602, 0xc4e603913394eb11, 0xd31cfcdcd9c4cdb2,
     0xa164d352b18499c4, 0x82f35ee1f69f9f8e, 0x3660c07bad4b60af,
     0x69de925622c194c6, 0xaad25dd9c221c271, 0x4cae3e00b8c4d68,
     0xa717f1200b3b94b2, 0x31a0e879e2535099, 0x5defaffe64a1e6a4,
     0x50b9404aedb0ba8e},
    /* after round 12 */
    {0x4089b1d2ef5d3a26, 0x8409f6c9be239ecb, 0x12657d5a4568ead5,
     0x4d9f3e32387e66d3, 0x3be7f6680bb1c847, 0x61ed7fb0ce6ca6af,
     0x3671348a3fbd6487, 0x81e9f633f01dcf03, 0xab5eefa38e5546d9,
     0x8298d4e430062494, 0x3229e84a8e1704c2, 0x56f221867a1b3140,
     0xdb7043cf08757bb2, 0x52de6de69935b888, 0x98a1c2d38194eda1,
     0x9af85d4549fb53c7, 0x667e09de6b59e740, 0xc08877b3d1a7a9f3,
     0x5da108d19209739f, 0xe0e096ffe4ff71ea, 0x77badafbfd43d602,
     0x50a13329b93c785f, 0xf8ae145adffbe076, 0x366d5164bb8f0bb5,
     0x438298aa08f16a54},
    /* after round 13 */
    {0x648de89c6833c102, 0x656c92d6905c3423, 0x29f78ba152939175,
     0x915f4042be658083, 0x1ef11c5578706c33, 0x1322254abe46649e,
     0x7aa01437721057c0, 0xa1ffa1e15ff0bd68, 0xfaf58b3abf5862df,
     0xb22b9af8df7d5fe7, 0x4e6245814643330b, 0x2c931b528b695b9,
     0x694b851d604bfa01, 0x91010d55fa046ca,  0x1acec2cda2b1c6c6,
     0x329a908710cd5a85, 0xa89186e775f2fb74, 0xee1b3818d5a83825,
     0x212da229e198439,  0x45428a665086ac41, 0xd7f5dbea9adc49ad,
     0x752f2fec3448e9b2, 0x8360adfb6db369d,  0xee429201d6040138,
     0x3c6534877469885f},
    /* after round 14 */
    {0xf204ab825ec1b98c, 0x32132932ed2960e,  0xd2e894a105ce76b0,
     0x75da76bd87d37a73, 0x8c6c05a408afc4ba, 0xae399b7b6c84633d,
     0xa8efb92313ac7e44, 0x83371dd98c8b09f1, 0x7cf0f6ddaf592ee4,
     0x8aad4677926b6a65, 0xf42645f1aeb80189, 0x41ff2cbc6c803fdd,
     0x88669871e9407470, 0xe5bbddf2601ea85b, 0x77129c370664d096,
     0x38f5de3ed1cbf84f, 0x99cedbf1933437f0, 0xde74aeb20a95db03,
     0xfa2d09f00f8b19be, 0x4754b7b9faeff7b,  0xb3606dbe9db6c471,
     0x31b09e98df7db202, 0xbe340816f1a99ed0, 0x1ea140c98ab9faef,
     0x9d52cdc98b2a671a},
    /* after round 15 */
    {0x6ee364e771b6bcf,  0xe658feaa402f4bd5, 0xd9c51e50969aab69,
     0x9473996fe1bb4ceb, 0xf51cb0626da6f085, 0xd2ea3cac414eb63b,
     0xf55fd4ddbf081e7d, 0xf666c3286f380b1d, 0xc9e47ebe6f50efb2,
     0xd19df82730ea16b5, 0x3fb7294d1438d9b,  0x40b5bb60943d81fe,
     0x65dee655c35a4195, 0x53e3c5f2892e6bdb, 0x908c33b016575e0b,
     0xf135e2a8960f74e2, 0x4c225dea370a687d, 0x46230e694915f13e,
     0x11c0069f4d346280, 0x50b4ad11e821c365, 0xa027d4e0e1cd84da,
     0x29ff28d8da171496, 0xe5800698cb3ae5b5, 0x637658693c47003f,
     0x8cbcfd35e82d3301},
    /* after round 16 */
    {0x294d0853009a2e9,  0x6d4b92a0afd4a76b, 0x86a9931dbbbe2820,
     0x5c3ce9b0ea86020e, 0x322c45cc79b07a6c, 0x2dfca2b3ca5eda71,
     0x8ca51c558e7b125f, 0x780c883422dea4d0, 0x3cefe3bf645dc61f,
     0xc54f3e2ef42a032e, 0x7e1ba84748e80bee, 0xc777b862349466ae,
     0x48edac5c5f026f97, 0x8e5c9db63e29f72a, 0xdc8e9ad663eb67f4,
     0xc3d4c4c0a4002291, 0x6cdaf1c7e7ddf7c0, 0x70433fd50a2fca56,
     0x197659ff2edcc466, 0xcf709311541f6f05, 0x189c40775351dcb,
     0x39f99b067422b547, 0x747a4ce4ebd89471, 0x2bd80f68cc4e6008,
     0xc0c81d74edb1b855},
    /* after round 17 */
    {0x5d350f71494fc8f7, 0xe37bf573d8aafe2d, 0x26c80c388310b9ed,
     0xa284df955f5574b8, 0x3807beb7e976fca5, 0x2f3b2ad03fba7fb4,
     0xb6ea17c2a22bcb5e, 0x8e03bdc59cf78585, 0x982733e2d63b636a,
     0x7734677d193a2578, 0x3525810e4ec2258f, 0xf1d6dfc015c6d99d,
     0xe264abbddcf60d8e, 0x3ce653f16dc02838, 0x87bcb30fe85bd280,
     0x9c4e438b1642898e, 0x10c59ecbc76705e4, 0x6fb9736051442cc0,
     0x3c9b4bf3af2dc5d,  0x84501b4167d1303f, 0x95407746b76fa5ee,
     0xe9e1987c2ef60a70, 0x5ff558f3797f4c78, 0x2e9689056e4699b8,
     0x70702cd96a9e15bb},
    /* after round 18 */
    {0xfbdf32a8511a215a, 0x77cc8c29c4070d1,  0xa1d7456dd43b2098,
     0x628b689b07200e95, 0x67a8fa7ccafb6fcc, 0x4bc8b925db11fc4e,
     0x795330566efd965c, 0x12e5367ba3ec4777, 0x6256066085a1c509,
     0x9a956cbbb31fa973, 0x91282d2c07d3c3ea, 0x55c1963ad365f063,
     0x471ab241a976b335, 0x8099b9e1f6f6e869, 0x321a2cfbb72874a4,
     0xf35acbbfb66c85ca, 0x56b6b30b89e00255, 0x84b0356a433bf9a8,
     0x853d1f0e8d821215, 0x508c1b026dafdc95, 0x7345ae61eb7c80a2,
     0x87ccc9784dd9d4bf, 0x3a624359f6a8b2b5, 0x6da5463417ce143f,
     0xc801eff982630994},
    /* after round 19 */
    {0x599d6ef52fd9ae41, 0xbf34c0d029d604e5, 0x88d5a13bfc74406,
     0xfc80d28cc5cc542e, 0x3be9b37a46dfa823, 0x8bf1357228f2736e,
     0x66c66703bb4d90a6, 0xdf7213b7f0a141c9, 0xb409a8ca23c781ef,
     0xa627237129eee7a1, 0x62e2cafa15d2e5b2, 0x720fede69ef73316,
     0x33db8baeaecb6f23, 0x5211152025d6cffa, 0x347937f914e65553,
     0x5ce481e8d139a791, 0x9330bdde8ca5f7d9, 0x10ee357fe5674585,
     0x8ed9bdd71948545e, 0xcd893f7adababc47, 0xa20e907479eb38fc,
     0x63076cb5f51fcd4b, 0xc1932a104f10527a, 0x7c5fb2e3e371aef9,
     0x9124ff3f29c7839c},
    /* after round 20 */
    {0x1a1ad63ea6de94cd, 0x76f7142d14da9bfc, 0xa59d6cd06eb2ac2e,
     0xe3b85e2e2fccb614, 0xc36e991c1a383054, 0x689cbc39b01c0cb9,
     0x7a0765a0cd8d0b02, 0x8b60d78abfae9861, 0x35f1f6fe72e0a82f,
     0x8f4eeff944e0c2d4, 0x632b5576588a026d, 0x2b12597baf864e64,
     0x555eb1d169fa7808, 0x6c5d7fd109030bd9, 0x5ec69b93ee6cf97,
     0x1852407bda60f6dc, 0xab561f9c9be28b10, 0xabfe34cd40bb334a,
     0x371ab1a61efdcd8a, 0x58a7ac24ec3ffe99, 0xaa96c5da7975161a,
     0x21b79a41e0bc679b, 0xa8fc47e37729343d, 0x5decaa3cdbd5c36e,
     0x4b60687e620ae2b4},
    /* after round 21 */
    {0xcb967722d7e71945, 0x8e72c3b2db12a66f, 0xeff0aac9862e6863,
     0xcbcfe6fa8917f50f, 0xf0388aed43035577, 0x76c4ebd02adac11a,
     0x88f1abd844d153fd, 0x93ca1652def39a7,  0xafbd5eafaa200412,
     0x3cb75a7d2f1410e1, 0x5bbcb96cfede2e86, 0xedb9f9068bf73fef,
     0xf29ee305774f5d41, 0xdf8594cb00c06448, 0xb22a75014d2fbb08,
     0x96cf2f872c22d061, 0x50bcac8b9823c48c, 0xced2c9104a8bc9a6,
     0x1064930f71891b9d, 0x6a81ca651589b15e, 0xa256c27254a4eaa6,
     0xf79babacba12d714, 0xa980d201aecc693b, 0xcb631a23adca9452,
     0xdbdb6ee4e7362dc1},
    /* after round 22 */
    {0x1dd2da9769da011f, 0x3e2c73cc1842b6ee, 0x11acb1eb1af15940,
     0xdacf89ea70fddb42, 0x76cd1ada7bddb391, 0x3329889348e3611e,
     0x79822e30740f8a94, 0x274f2636a613dacd, 0x9b74448952711318,
     0x841de7cdc8f8ba3d, 0xf129d553258ac6c9, 0x2ade13fd70a45485,
     0x843ea2b8afe2f08b, 0xdef7504ff47bd92e, 0xbfc5ae08ed82cf6b,
     0x604b94ba87023ffa, 0x55660ce1001cc5f9, 0x7e78ea0caeaf0b0f,
     0x84f23e7133a9197,  0x5c0794ebaa3e4dfd, 0xb09f4bd1d370e12c,
     0x50a2a1d79f1ef2b3, 0x2ba8d5e7db313e6a, 0xcc0258a30644e223,
     0xb96b2d8cb9b1e79},
    /* after round 23 */
    {0xe2e4980d944da77,  0x3c4dfa24420069f,  0x4247f8944d4ae7fe,
     0x602c35affe556148, 0xd8f2b86d8510572f, 0x4a77bfa126fc093,
     0x3ddead3d8c1401b7, 0xea0ddc0d9366302,  0x262012b5083c91ac,
     0xe7fb7b40b7e90583, 0xffb9fc9a0f77452e, 0x76004ea89b563e26,
     0xd3d0c5789e1909a3, 0x16e2aa8036b5013d, 0xe249aeb39c95d7db,
     0x58c5f58b0b1e40ce, 0xe1f0b375845fbd47, 0xd7175255383fc27,
     0xf235e528401d5007, 0xc52e05ccf15f628f, 0x73cca4ae07da389d,
     0xd6a59cd027a68607, 0x867edb7567fc04c7, 0x9d2fe354084dd32a,
     0xcadbab8c23a95c88},
    /* after round 24 */
    {0xf1258f7940e1dde7, 0x84d5ccf933c0478a, 0xd598261ea65aa9ee,
     0xbd1547306f80494d, 0x8b284e056253d057, 0xff97a42d7f8e6fd4,
     0x90fee5a0a44647c4, 0x8c5bda0cd6192e76, 0xad30a6f71b19059c,
     0x30935ab7d08ffc64, 0xeb5aa93f2317d635, 0xa9a6e6260d712103,
     0x81a57c16dbcf555f, 0x43b831cd0347c826, 0x1f22f1a11a5569f,
     0x5e5635a21d9ae61,  0x64befef28cc970f2, 0x613670957bc46611,
     0xb87c5a554fd00ecb, 0x8c3ee88a1ccf32c8, 0x940c7922ae3a2614,
     0x1841f924a2c509e4, 0x16f53526e70465c2, 0x75f644e97f30a13b,
     0xeaf1ff7b5ceca249},
    /* final result */
    {0xf1258f7940e1dde7, 0x84d5ccf933c0478a, 0xd598261ea65aa9ee,
     0xbd1547306f80494d, 0x8b284e056253d057, 0xff97a42d7f8e6fd4,
     0x90fee5a0a44647c4, 0x8c5bda0cd6192e76, 0xad30a6f71b19059c,
     0x30935ab7d08ffc64, 0xeb5aa93f2317d635, 0xa9a6e6260d712103,
     0x81a57c16dbcf555f, 0x43b831cd0347c826, 0x1f22f1a11a5569f,
     0x5e5635a21d9ae61,  0x64befef28cc970f2, 0x613670957bc46611,
     0xb87c5a554fd00ecb, 0x8c3ee88a1ccf32c8, 0x940c7922ae3a2614,
     0x1841f924a2c509e4, 0x16f53526e70465c2, 0x75f644e97f30a13b,
     0xeaf1ff7b5ceca249}};

int test_round(int nr, uint64_t* state) {
  int x, y;
  for (y = 0; y < 5; ++y) {
    for (x = 0; x < 5; ++x) {
      if (expected[nr][y * 5 + x] != state[y * 5 + x]) {
        return -1;
      }
    }
  }

  return 0;
}

int test_permutation_rounds(void) {
  uint64_t A[25]; /* keccak-f[1600] */

  /* zero out state */
  int i, j;
  for (j = 0; j < 5; ++j) {
    for (i = 0; i < 5; ++i) {
      A[j * 5 + i] = 0;
    }
  }

  keccak_permutation(A);

  printf("Final result:\n");

  if (test_round(24, A) == -1) {
    printf("TEST FAILED\n");
  } else {
    printf("TEST OK!\n");
  }

  return 0;
}

typedef struct test_vec_t {
  unsigned char* msg;
  uint8_t md[32];
} test_vec_t;

int test_digest_bytes(void) {
  test_vec_t test_vectors[3] = {
      {(unsigned char*)"SHA3-256 Test Hash",
       {0xb9, 0x55, 0x02, 0x7e, 0x6a, 0x08, 0xcb, 0xee, 0xbe, 0xa8, 0x5c,
        0x3b, 0x91, 0xa0, 0x37, 0x30, 0x6e, 0x49, 0xe1, 0x7f, 0x36, 0x93,
        0x99, 0xa4, 0x80, 0x58, 0x01, 0x88, 0xb2, 0x50, 0x0b, 0xa8}},
      {(unsigned char*)"A quick brown fox jumps over the lazy dog",
       {0x2b, 0xaa, 0x15, 0xb5, 0xa2, 0x04, 0xf7, 0x4a, 0xe7, 0x08, 0xd5,
        0x88, 0x79, 0x36, 0x57, 0xa7, 0x0c, 0xda, 0x22, 0x88, 0xa0, 0x6e,
        0x7e, 0x12, 0xc9, 0x18, 0xcc, 0x3a, 0xed, 0xc5, 0xcd, 0x8d}},
      {(unsigned char*)"Lorem Ipsum",
       {0x8c, 0x1b, 0xf4, 0xfd, 0x81, 0xa1, 0x87, 0xa3, 0xb6, 0x16, 0x70,
        0x0a, 0x6c, 0x9b, 0x50, 0xb4, 0xbe, 0x51, 0x02, 0xe4, 0x7b, 0x1d,
        0x21, 0x15, 0xe0, 0xa9, 0xde, 0xc3, 0x64, 0x80, 0x18, 0x28}}};
  int i, fails;

  fails = 0;
  for (i = 0; i < 3; ++i) {
    test_vec_t test = test_vectors[i];

    printf("Testing test [%d], input length: %d bytes\n", i,
           (int)strlen((const char*)test.msg));

    unsigned char* result =
        keccak_hash(test.msg, strlen((const char*)test.msg));

    if (memcmp(result, test.md, 32)) {
      printf("Test %d failed: Expected output does not match result.\n", i + 1);
      printf("Expected:\n");
      print_digest(test.md);
      printf("Result:\n");
      print_digest(result);
      fails++;
      continue;
    }
    free(result);
    printf("Test %d OK\n", i + 1);
  }

  return fails;
}

int main(void) {
  if (test_permutation_rounds() == -1) {
    return -1;
  }

  if (test_digest_bytes() == -1) {
    return -1;
  }
  return 0;
}
