#ifndef NETWORKBLOCKER_HPP
#define NETWORKBLOCKER_HPP

#include <jni.h>
#include <string>
#include <vector>
#include <unordered_set>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <dlfcn.h>

inline struct hostent* (*orig_gethostbyname)(const char*) = nullptr;
inline int (*orig_getaddrinfo)(const char*, const char*, const struct addrinfo*, struct addrinfo**) = nullptr;

class NetworkBlocker {
private:
    static std::unordered_set<std::string> blockedDomains;
    static std::unordered_set<std::string> whitelistedDomains;
    static std::unordered_set<std::string> blockedIPs;
    // [FIX] Cache keyword set — cek sekali di luar loop
    static std::unordered_set<std::string> blockedKeywords;
    static bool isInitialized;
    static bool hooksActive;

public:
    static void initialize() {
        if (isInitialized) return; // [FIX] hapus LOGI "already initialized" — ga perlu
        
        whitelistedDomains = {
            "g83naxx1ena.gph.easebar.com",
            "mgbsdknaeast.matrix.easebar.com",
            "g83naxx1ena.update.easebar.com"
        };

        // [FIX] keyword dipisah ke set sendiri — cek sekali di luar loop isDomainBlocked
        blockedKeywords = {
            "bloodstrike",
            "blood-strike",
        };
        
        blockedIPs = {
            "13.248.222.62","142.251.135.142","18.161.49.13","185.199.110.153",
            "23.198.104.192","23.210.96.164","34.120.31.123","34.124.164.139",
            "34.124.204.207","34.126.73.6","34.142.221.235","34.142.239.151",
            "34.143.130.186","34.143.137.33","34.143.185.38","34.143.187.29",
            "34.143.192.140","34.143.221.203","34.84.94.246","34.87.159.190",
            "34.87.186.106","35.198.219.207","35.198.226.62","35.199.90.239",
            "35.240.133.108","35.240.152.172","35.240.201.242","35.240.238.247",
            "35.72.104.232","47.242.206.246","76.223.88.1","8.218.96.148",
            "108.177.111.77","108.177.111.78","108.177.111.98","108.177.113.121",
            "108.177.113.89","108.177.115.99","108.177.116.75","108.177.119.78",
            "108.177.120.45","108.177.122.90","108.177.123.176","108.177.123.56",
            "108.177.123.62","108.177.123.67","108.177.125.34","108.177.125.76",
            "108.177.125.91","108.177.126.88","108.177.127.19","108.177.127.68",
            "108.177.136.98","108.177.138.99","108.177.8.194","108.177.98.24",
            "108.177.98.53","108.177.98.56","108.178.112.88","108.178.122.98",
            "108.178.123.99","108.178.124.72","108.178.126.98","108.178.134.82",
            "108.178.135.101","108.178.135.83","108.178.145.111","108.178.145.83",
            "108.178.146.112","108.179.109.83","108.179.110.84","108.179.123.131",
            "108.179.133.131","108.179.134.132","108.179.99.73","142.250.12.92",
            "142.250.120.55","142.250.121.89","142.250.122.35","142.250.128.101",
            "142.250.128.155","142.250.182.141","142.250.184.82","142.250.185.78",
            "142.250.189.104","142.250.19.78","142.250.19.79","142.250.191.46",
            "142.250.34.127","142.250.45.149","142.250.45.158","142.250.56.190",
            "142.250.64.78","142.250.68.184","142.250.69.98","142.250.77.99",
            "142.250.78.195","142.250.88.194","142.250.98.110","142.250.98.111",
            "142.250.98.128","142.251.130.65","142.251.150.65","142.251.151.66",
            "142.251.78.194","142.252.138.165","142.252.148.175","142.252.149.176",
            "142.253.55.159","142.253.65.169","142.253.66.170","142.254.29.89",
            "142.254.39.99","142.254.40.100","142.255.108.120","142.255.118.130",
            "142.255.119.131","172.217.11.174","172.217.12.119","172.217.14.238",
            "172.217.14.66","172.217.15.95","172.217.19.194","172.217.20.61",
            "172.217.22.34","172.217.25.111","172.217.28.45","172.217.28.48",
            "172.217.28.88","172.217.30.119","172.217.35.131","172.217.35.55",
            "172.217.35.58","172.217.35.91","172.217.36.132","172.217.45.174",
            "172.217.50.80","172.217.50.85","172.217.55.114","172.217.55.142",
            "172.217.6.14","172.217.60.92","172.217.9.202","172.218.16.105",
            "172.218.25.121","172.218.26.125","172.218.27.126","172.218.65.152",
            "172.218.66.153","172.219.29.58","172.219.36.68","172.219.39.68",
            "172.219.40.69","172.219.46.78","172.219.47.79","172.219.51.90",
            "172.219.55.152","172.219.61.100","172.219.62.101",
            "2407:0:0:29b::f4d","2407:0:0:29b::f4e","2407:0:0:29b::f4f",
            "2407:0:0:29b::f50","2407:0:0:29b::f51","2407:0:0:29b::f52",
            "2407:0:0:29b::f53","2407:0:0:29b::f54","2407:0:0:29b::f55",
            "2407:0:1000:1::7204:27c8","2407:0:1000:1::7204:27d1"
        };
        
        blockedDomains = {
            "analytics.bloodstrike.netease.com","bamreport.bloodstrike.netease.com",
            "reportban.netease.com","ban.netease.com",
            "matchreport.bloodstrike.netease.com","securitycheck.netease.com",
            "accountsecurity.netease.com","userlog.bloodstrike.netease.com",
            "qsec.netease.com","tinfo.netease.com","log.qos.netease.com",
            "tracking.netease.com","anticheat.bloodstrike.net",
            "anticheat-api.bloodstrike.com","report.bloodstrike.net",
            "report-api.bloodstrike.com","report-api.bloodstrike.io",
            "report-api.bloodstrike.net","appeal-api.bloodstrike.io",
            "log.bloodstrike.net","track.bloodstrike.io",
            "log-collector.bloodstrike.com","stat-api.bloodstrike.net",
            "update-api.bloodstrike.io","config.bloodstrike.io",
            "easebar.com","monitor.easebar.com","report.easebar.com",
            "log.easebar.com","analytics.easebar.com","telemetry.easebar.com",
            "crash.easebar.com","metrics.easebar.com","track.easebar.com",
            "event.easebar.com","api.easebar.com","data.easebar.com",
            "push.easebar.com","stat.easebar.com","perf.easebar.com",
            "tp2.qq.com","msdk.qq.com","beacon.qq.com","aegis.qq.com",
            "report.mta.qq.com","log.mta.qq.com","stat.mta.qq.com",
            "crash.mta.qq.com","track.mta.qq.com","analytics.mta.qq.com",
            "data.mta.qq.com","bugly.qq.com","report.bugly.qq.com",
            "log.bugly.qq.com","android.bugly.qq.com","upload.crashsight.qq.com",
            "crashsight.qq.com","speedtrace.qq.com","trace.qq.com",
            "databus.qq.com","report.databus.qq.com","log.databus.qq.com",
            "ams.netease.com","report.ams.netease.com","log.ams.netease.com",
            "stat.ams.netease.com","track.ams.netease.com","telemetry.netease.com",
            "crash.netease.com","perf.netease.com","metrics.netease.com",
            "data.netease.com","event.netease.com","push.netease.com",
            "log.netease.com","analytics.netease.com","monitor.netease.com",
            "report.netease.com","cdn-reporting.netease.com","diagnose.netease.com",
            "neteaseace.com","report.neteaseace.com","log.neteaseace.com",
            "stat.neteaseace.com","api.neteaseace.com","check.neteaseace.com",
            "scan.neteaseace.com","anticheat.neteaseace.com","protect.neteaseace.com",
            "bd.ad.netease.com","crash.ad.netease.com","report.ad.netease.com",
            "log.ad.netease.com","stat.ad.netease.com","track.ad.netease.com",
            "analytics.ad.netease.com","telemetry.ad.netease.com",
            "tprt.finclip.com","log.finclip.com","event.finclip.com",
            "tns.baikemaps.com","log.baikemaps.com","report.baikemaps.com",
            "dc.services.visualstudio.com","mobile.events.data.microsoft.com",
            "api2.amplitude.com","cdn.amplitude.com","api.segment.io",
            "cdn.segment.io","api.mixpanel.com","cdn.mxpnl.com",
            "app.adjust.com","s2s.adjust.com","app.appsflyer.com",
            "t.appsflyer.com","e.appsflyer.com","log.byteoversea.com",
            "log.byteintl.com","log.tiktokv.com","mssdk-oversea.byteoversea.com",
            "mssdk-us-east.byteoversea.com","mon.byteoversea.com",
            "report.snssdk.com","log.snssdk.com","is.snssdk.com",
        };
        
        isInitialized = true;
        hooksActive   = true;
        LOGI("NetworkBlocker initialized: %zu domains, %zu IPs",
             blockedDomains.size(), blockedIPs.size());
    }

    static bool isHooksActive() { return hooksActive; }
    static void setHooksActive(bool v) { hooksActive = v; }

    static void enable_domain_blocking() {
        if (!isInitialized) initialize();
        setHooksActive(true);
    }
    static void disable_domain_blocking() { setHooksActive(false); }
    static bool is_domain_blocking_enabled() { return isHooksActive(); }

    // [FIX] isDomainWhitelisted — hapus loop manual duplikat, pakai find() aja
    static bool isDomainWhitelisted(const std::string& clean_host) {
        if (clean_host.empty()) return false;
        // Direct match
        if (whitelistedDomains.count(clean_host)) return true;
        // Suffix match: cek apakah clean_host adalah subdomain dari whitelist
        for (const auto& w : whitelistedDomains) {
            if (clean_host.size() > w.size() + 1) {
                size_t off = clean_host.size() - w.size();
                if (clean_host[off - 1] == '.' && clean_host.compare(off, w.size(), w) == 0)
                    return true;
            }
        }
        return false;
    }

    // [FIX] isIPBlocked — hapus LOGI dari hot path, hanya log di caller kalau perlu
    static bool isIPBlocked(const std::string& ip) {
        if (ip.empty()) return false;
        if (ip == "127.0.0.1" || ip == "::1" || ip == "0.0.0.0") return false;
        if (blockedIPs.count(ip)) return true;
        // CIDR prefix check (jarang, tapi tetap support)
        for (const auto& b : blockedIPs) {
            size_t slash = b.find('/');
            if (slash != std::string::npos) {
                if (ip.compare(0, slash, b, 0, slash) == 0) return true;
            }
        }
        return false;
    }

    // [FIX] isDomainBlocked — 3 fix:
    // 1. Hapus LOGI dari setiap match (blocking I/O per network call)
    // 2. Keyword check dipindah ke LUAR loop (sebelumnya dicek 200x per call)
    // 3. Suffix match pakai rfind bukan substr (lebih efisien)
    static bool isDomainBlocked(const std::string& hostname) {
        if (!isInitialized) initialize();
        if (!hooksActive || hostname.empty()) return false;

        // Strip trailing dots
        std::string h = hostname;
        while (!h.empty() && h.back() == '.') h.pop_back();
        if (h.empty()) return false;

        if (isDomainWhitelisted(h)) return false;
        if (isIPBlocked(h)) return true;

        // [FIX] Keyword check SEKALI di luar loop — bukan tiap iterasi domain
        for (const auto& kw : blockedKeywords) {
            size_t pos = h.find(kw);
            if (pos != std::string::npos && (pos == 0 || h[pos - 1] == '.'))
                return true;
        }

        // Domain/subdomain match
        if (blockedDomains.count(h)) return true;
        for (const auto& d : blockedDomains) {
            if (h.size() > d.size() + 1) {
                size_t off = h.size() - d.size();
                if (h[off - 1] == '.' && h.compare(off, d.size(), d) == 0)
                    return true;
            }
        }

        return false;
    }

    // [FIX] gethostbyname_hooked — hapus LOGI "called: %s" dari hot path
    static struct hostent* gethostbyname_hooked(const char* name) {
        if (!name) return orig_gethostbyname ? orig_gethostbyname(name) : nullptr;

        if (isHooksActive() && isDomainBlocked(name)) {
            // Hanya log sekali kalau benar-benar diblock
            LOGI("NB: block gethostbyname %s", name);
            static struct hostent  fakeHost;
            static struct in_addr  fakeAddr;
            static char*           fakeAliases[1]  = {nullptr};
            static char*           fakeAddrList[2] = {nullptr, nullptr};
            static char            fakeName[256];
            strncpy(fakeName, name, sizeof(fakeName) - 1);
            fakeName[sizeof(fakeName) - 1] = '\0';
            fakeAddr.s_addr       = inet_addr("127.0.0.1");
            fakeHost.h_name       = fakeName;
            fakeHost.h_aliases    = fakeAliases;
            fakeHost.h_addrtype   = AF_INET;
            fakeHost.h_length     = sizeof(struct in_addr);
            fakeHost.h_addr_list  = fakeAddrList;
            fakeHost.h_addr_list[0] = (char*)&fakeAddr;
            fakeHost.h_addr_list[1] = nullptr;
            return &fakeHost;
        }
        return orig_gethostbyname ? orig_gethostbyname(name) : nullptr;
    }

    // [FIX] getaddrinfo_hooked — hapus LOGI "called: %s" dari hot path
    static int getaddrinfo_hooked(const char* node, const char* service,
                                  const struct addrinfo* hints, struct addrinfo** res) {
        if (!node) return orig_getaddrinfo ? orig_getaddrinfo(node, service, hints, res) : EAI_NONAME;

        if (isHooksActive() && isDomainBlocked(node)) {
            LOGI("NB: block getaddrinfo %s", node);
            if (res) {
                auto* fake = (struct addrinfo*)  malloc(sizeof(struct addrinfo));
                auto* addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
                if (!fake || !addr) { free(fake); free(addr); return EAI_MEMORY; }
                memset(fake, 0, sizeof(*fake));
                memset(addr, 0, sizeof(*addr));
                addr->sin_family      = AF_INET;
                addr->sin_addr.s_addr = inet_addr("127.0.0.1");
                addr->sin_port        = service ? htons(atoi(service)) : 0;
                fake->ai_family    = AF_INET;
                fake->ai_socktype  = hints ? hints->ai_socktype : SOCK_STREAM;
                fake->ai_protocol  = hints ? hints->ai_protocol : IPPROTO_TCP;
                fake->ai_addrlen   = sizeof(struct sockaddr_in);
                fake->ai_addr      = (struct sockaddr*)addr;
                fake->ai_canonname = nullptr;
                fake->ai_next      = nullptr;
                *res = fake;
                return 0;
            }
            return EAI_NONAME;
        }
        return orig_getaddrinfo ? orig_getaddrinfo(node, service, hints, res) : EAI_SYSTEM;
    }
};

std::unordered_set<std::string> NetworkBlocker::blockedDomains;
std::unordered_set<std::string> NetworkBlocker::whitelistedDomains;
std::unordered_set<std::string> NetworkBlocker::blockedIPs;
std::unordered_set<std::string> NetworkBlocker::blockedKeywords;
bool NetworkBlocker::isInitialized = false;
bool NetworkBlocker::hooksActive   = false;

extern "C" {
    void enable_domain_blocking()       { NetworkBlocker::enable_domain_blocking(); }
    void disable_domain_blocking()      { NetworkBlocker::disable_domain_blocking(); }
    bool is_domain_blocking_enabled()   { return NetworkBlocker::is_domain_blocking_enabled(); }
}

#endif // NETWORKBLOCKER_HPP
