#pragma once
// getopt handling
namespace {
    const string helpLine = "Usage: speck-command [-etxrh] <command> [<args>]";
    const string helpContents = 
        "Available commands are:\n"
        "   keygen          Generate a (random) key for a given key size\n"
        "   encrypt         Encrypt a single block given as an argument\n"
        "   decrypt         Decrypt a single block given as an argument\n"
        "   encrypt-stream  Encrypt a stream of given in stdin\n"
        "   decrypt-stream  Decrypt a stream of data given in stdin\n\n"
        "Run with '<command> -h' to see command specific help.";

    const string keygenLine =
        "Usage: speck-command keygen [KEYBITS]\n"
        "Generate a random key given a key size, and output the key as a hexadecimal\n"
        "number.\n\n"
        "   KEYBITS         number of bits in the generated key. Default 256\n"
        "                   Valid bit sizes are 64, 72, 96, 128, 144, 192, 256.";

    const string helpModes =
        "   -h, --hex (default)    accept/generate PLAINTEXT and CIPHERTEXT in hex mode.\n"
        "                          hex = hexadecimal number (implies big-endian).\n"
        "                          No spaces are allowed in between words.\n"
        "                          If shorter than a block, the input will be padded\n"
        "                          with zeros ON THE MSB (left).\n"
        "   -r, --text             accept/generate PLAINTEXT in text mode.\n"
        "                          CIPHERTEXT (generated or input) is still in hex mode\n"
        "                          KEY (input) is still in hex mode\n"
        "                          If shorter than a block, the input will be padded\n"
        "                          with zeros ON THE LSB (right).\n"
        "                          Upon decryption, the padding zeros will be stripped\n"
        "                          if under text mode";
    const string helpSizes = 
        "   -b, --block-size SIZE  block size in bits. Default is 128\n"
        "                          Valid block sizes are 32, 48, 64, 96, 128.\n"
        "   -k, --key-size SIZE    key size in bits. Default is 256\n"
        "                          Valid key sizes are 64, 72, 96, 128, 144, 192, 256.\n"
        "\n"
        "Valid (block-size, key-size) pairs are:\n"
        "(32, 64), (48, 72), (48, 96), (64, 96), (64, 128), (96, 96), (96, 144)\n"
        "(128, 128), (128, 192), (128, 256)";

    const string encryptHelpLine = 
        "Usage: speck-command encrypt KEY PLAINTEXT\n"
        "Encrypt a single PLAINTEXT block using Speck, with the key KEY.\n"
        "The resulting CIPHERTEXT block will be printed to stdout\n"
        "The PLAINTEXT block must not be longer than the block size.\n\n"
        "   KEY                    the key as a hexadecimal number\n"
        "   PLAINTEXT              the plaintext. See --text, --hex for modes";

    const string decryptHelpLine = 
        "Usage: speck-command decrypt KEY CIPHERTEXT\n"
        "Decrypt a single CIPHERTEXT block using Speck, with the key KEY.\n"
        "The resulting PLAINTEXT will be printed to stdout\n"
        "The CIPHERTEXT block must not be longer than the block size.\n\n"
        "   KEY                    the key as a hexadecimal number\n"
        "   CIPHERTEXT             the ciphertext.";

    const string encryptStreamHelp = 
        "Usage: speck-command encrypt-stream KEY\n"
        "Encrypt a stream of binary data from stdin, and write the encrypted\n"
        "result to stdout";
    const string decryptStreamHelp = 
        "Usage: speck-command decrypt-stream KEY\n"
        "Decrypt a stream of binary data from stdin, and write the decrypted\n"
        "result to stdout";
    const string streamHelp =
        "The stdin input is treated as consecutive blocks of big-endian binary\n"
        "packed data blocks, in the byte order of test vectors shown in \n"
        "https://eprint.iacr.org/2013/404.pdf\n\n"
        "   KEY                    the key as a hexadecimal number\n"
        "   -e, --ecb              process the stream in ecb mode. The stream size\n"
        "                          must be an exact multiple of the block size\n"
        "   -t, --tls-ecb          process the stream in ecb mode with tls padding\n"
        "                          see http://www.hit.bme.hu/~buttyan/courses/\n"
        "                          BMEVIHIM132/2010/04_block_enc_modes.pdf slide 9"
        ;

    const vector<string> commands = {
        "keygen", "encrypt", "decrypt", "encrypt-stream", "decrypt-stream",
    };
    const struct option long_options[] = {
        {"ecb",         no_argument,  0,  'e'},
        {"tls-ecb",     no_argument,  0,  't'},
        {"hex",         no_argument,  0,  'x'},
        {"text",        no_argument,  0,  'r'},
        {"help",        no_argument,  0,  'h'},
        {"block-size",  required_argument, 0, 'b'},
        {"key-size",    required_argument, 0, 'k'},

    };

    const char*         short_options = "etxrhb:k:";

    struct Options {
        vector<string> positionals;
        string command;
        bool ecb;
        bool tlsecb;
        bool hex;
        bool text;
        bool help;
        int blockSize;
        int keySize;

        Options()
        : ecb(false), tlsecb(false), hex(false), text(false), help(false),
          blockSize(128), keySize(256) {}
    };

    Options parseOptions(int argc, char* argv[]) {
        optind = 1;
        Options options;

        while(true) {
            int result = getopt_long(argc, argv, short_options, long_options, NULL);
            if (result == -1) {
                while (optind < argc) {
                    options.positionals.push_back(argv[optind++]);
                }
                break;
            }

            string opt;
            if (optarg != nullptr) {
                opt = optarg;
            }

            switch (result) {
                case 'h':
                    options.help = true;
                    break;
                case 'e':
                    options.ecb = true;
                    break;
                case 't':
                    options.tlsecb = true;
                    break;
                case 'x':
                    options.hex = true;
                    break;
                case 'r':
                    options.text = true;
                    break;
                case 'b':
                    options.blockSize = atoi(opt.c_str());
                    break;
                case 'k':
                    options.keySize = atoi(opt.c_str());
                    break;
                default:
                    cerr << helpLine << endl;
                    exit(1);
            }
        }

        bool fail = false;

        if (options.positionals.size() > 0) {
            options.command = options.positionals[0];
            options.positionals.erase(options.positionals.begin());
        } else if (!options.help) {
            cerr << argv[0] << ": no command specified." << endl;
            fail = true;
        }

        if (!fail && options.command != "" &&
            find(commands.begin(), commands.end(), options.command) ==
                commands.end()) {
            cerr << argv[0] << ": invalid command " << options.command << endl;
            fail = true;
        }

        if (!options.help && !fail) {
            if (!options.text) {
                options.hex = true;
            }

            if (options.text && options.hex) {
                cerr << argv[0] 
                    << ": you can only specify one of --hex and --text" << endl;
                fail = true;
            }

            if (options.ecb && options.tlsecb) {
                cerr << argv[0]
                     << ": you can only specify one of --ecb and --tls-ecb"
                     << endl;
                fail = true;
            }

            if (options.command.find("-stream") != string::npos) {
                if (!options.ecb && !options.tlsecb) {
                    cerr << argv[0]
                         << ": you must specifiy either --ecb or --tls-ecb "
                            "for stream mode"
                         << endl;
                    fail = true;
                }
            }
        }

        if (fail && !options.help) {
            cerr << helpLine << endl;
            exit(1);
        }

        if (options.help) {
            if (options.command == "keygen") {
                cerr << keygenLine << endl;
            } else if (options.command == "encrypt") {
                cerr << encryptHelpLine << endl;
                cerr << helpModes << endl << helpSizes << endl;
            } else if (options.command == "decrypt") {
                cerr << decryptHelpLine << endl;
                cerr << helpModes << endl << helpSizes << endl;
            } else if (options.command == "encrypt-stream") {
                cerr << encryptStreamHelp << endl;
                cerr << streamHelp << endl << helpSizes << endl;
            } else if (options.command == "decrypt-stream") {
                cerr << decryptStreamHelp << endl;
                cerr << streamHelp << endl << helpSizes << endl;
            } else {
                cerr << helpLine << endl;
                cerr << helpContents << endl;
            }
            if (!fail) {
                exit(0);
            } else {
                exit(1);
            }
        }

        return options;
    }
}