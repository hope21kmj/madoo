// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "core.h"
#include "protocol.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

//
// Main network
//

unsigned int pnSeed[] =
{
    0xd335d197, 0xd335d198,
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x3d;
        pchMessageStart[1] = 0xd5;
        pchMessageStart[2] = 0x5f;
        pchMessageStart[3] = 0xf9;
        vAlertPubKey = ParseHex("0437b4b0f5d356f205c17ffff6c46dc9ec4680ffb7f8a9a4e6eebcebd5f340d01df00ef304faea7779d97d8f1addbe1e87308ea237aae3ead96e0a736c7e9477a1");
        nDefaultPort = 22444;
        nRPCPort = 22443;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 18);
//        nSubsidyHalvingInterval = 210000;

        // PoSV
        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 18);
        nLastProofOfWorkHeight = 350 - 1;
        nStakeMinAge = 2 * 60 * 60; // 2 hours
        nStakeMaxAge = 14 * 24 *  60 * 60; // 14 days

        // Genesis Block:
        // CBlock(hash=12a765e31ffd4059bada, PoW=0000050c34a64b415b6b, ver=1, hashPrevBlock=00000000000000000000, hashMerkleRoot=97ddfbbae6, nTime=1317972665, nBits=1e0ffff0, nNonce=2084524493, vtx=1)
        //   CTransaction(hash=97ddfbbae6, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        //     CTxIn(COutPoint(0000000000, -1), coinbase 04ffff001d0104404e592054696d65732030352f4f63742f32303131205374657665204a6f62732c204170706c65e280997320566973696f6e6172792c2044696573206174203536)
        //     CTxOut(nValue=50.00000000, scriptPubKey=040184710fa689ad5023690c80f3a4)
        //   vMerkleTree: 97ddfbbae6
        const char* pszTimestamp = "January 17st 2019 was such a nice day...";
        CTransaction txNew(1546506552);
        txNew.nVersion = 1;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1500 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("041ac76ba3342f5d6be90912b906b45b616340b8fbcae7f74c329366175970c69761239ba48712669491fee2a350a80e2eeb5375fc93c55d8c628b1d71d775a9ca") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1546506552;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 222927360;

        hashGenesisBlock = genesis.GetHash();



        assert(hashGenesisBlock == uint256("0x23c5541b417a759214e04366a1d0d4da7eab33ff0e9e780a682d0109e84324ea"));
        assert(genesis.hashMerkleRoot == uint256("0x2ea399bf997b6f13ce649be8b8ead622696ce7e23a5255493685725613cccc79"));

        vSeeds.push_back(CDNSSeedData("madoocoin.com", "seed.madoocoin.com"));
//        vSeeds.push_back(CDNSSeedData("dnsseed01.redd.ink", "dnsseed01.redd.ink"));
//        vSeeds.push_back(CDNSSeedData("dnsseed02.redd.ink", "dnsseed02.redd.ink"));
//        vSeeds.push_back(CDNSSeedData("dnsseed03.redd.ink", "dnsseed03.redd.ink"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,189);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        // Convert the pnSeeds array into usable address objects.
        for (unsigned int i = 0; i < ARRAYLEN(pnSeed); i++)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7*24*60*60;
            struct in_addr ip;
            memcpy(&ip, &pnSeed[i], sizeof(ip));
            CAddress addr(CService(ip, GetDefaultPort()));
            addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
            vFixedSeeds.push_back(addr);
        }
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xf2;
        pchMessageStart[1] = 0x2e;
        pchMessageStart[2] = 0xe8;
        pchMessageStart[3] = 0x8a;
        vAlertPubKey = ParseHex("0403befbddb81d6c2b00626673ff0de907227785909d6844b82529fba7c81164b66ba10bb14157e27159e61b7b4c8dc0a36755289d930891b1507e9d23b852dd66");
        nDefaultPort = 35444;
        nRPCPort = 35443;
        strDataDir = "testnet3";
        nLastProofOfWorkHeight = 150 - 1; // Last POW block

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        // CTransaction txGenesis = genesis.vtx[0];
        genesis.nTime = 1546506522;
        // genesis.vtx[0] = txGenesis;
        genesis.nNonce = 3635302;
        hashGenesisBlock = genesis.GetHash();

	

        assert(hashGenesisBlock == uint256("0x3d214d299b327ea5e3d498bfc2ad73f6e5082d70db7116f986d180e30301108d"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("madoocoin.com", "testnet-seed.madoocoin.com"));
//        vSeeds.push_back(CDNSSeedData("redd.ink", "testnet-dnsseed.redd.ink"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,50);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0x33;
        pchMessageStart[1] = 0xcc;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0xee;
//        nSubsidyHalvingInterval = 150;
        bnProofOfWorkLimit = bnProofOfStakeLimit = CBigNum(~uint256(0) >> 1);
        nLastProofOfWorkHeight = 50 - 1;
        genesis.nTime = 1546506502;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 6;
        hashGenesisBlock = genesis.GetHash();



        nDefaultPort = 46444;
        strDataDir = "regtest";
        assert(hashGenesisBlock == uint256("0x93b1ec160ef44011f97f5349932e51319c4d8c39d1cf4dbf6c6da00a1c282014"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
