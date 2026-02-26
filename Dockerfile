FROM ghcr.io/therainbowphoenix/cp-codespace:beta-02

USER root

RUN set -eux; \
    # Remove any pre-existing APT source definitions (both .list and .sources)
    rm -f /etc/apt/sources.list; \
    rm -f /etc/apt/sources.list.d/*.list; \
    rm -f /etc/apt/sources.list.d/*.sources; \
    \
    # Write clean Bookworm sources
    printf "deb http://deb.debian.org/debian bookworm main contrib non-free non-free-firmware\n\
deb http://deb.debian.org/debian bookworm-updates main contrib non-free non-free-firmware\n\
deb http://deb.debian.org/debian-security bookworm-security main contrib non-free non-free-firmware\n" > /etc/apt/sources.list

RUN apt-get update -y && \
    apt-get install -y nano vim wget clangd clang-format && \
    rm -rf /var/lib/apt/lists/*


USER dev
# RUN wget https://raw.githubusercontent.com/ClasspadDev/faq/main/faq.md -O faq

RUN echo -e "echo \"$(tput setaf 2)\n\n [  > v<]~      Welcome to the ClassPad SDK ! \n                You can build using \"make bin\"\n                Check the faq \"cat ~/faq\"\n\n$(tput sgr0)\"" >> ~/.bashrc
