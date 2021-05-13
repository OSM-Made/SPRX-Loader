505:	
	@/bin/echo -e clear

	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=505
	+$(MAKE) -C Payload copy VERSION=505

	@/bin/echo -e "\e[1m\e[32m[5.05 Build Sucess!]\033[37m"

672:	
	@/bin/echo -e clear

	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=672
	+$(MAKE) -C Payload copy VERSION=672

	@/bin/echo -e "\e[1m\e[32m[6.72 Build Sucess!]\033[37m"

702:	
	@/bin/echo -e clear

	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=702
	+$(MAKE) -C Payload copy VERSION=702

	@/bin/echo -e "\e[1m\e[32m[7.02 Build Sucess!]\033[37m"

755:	
	@/bin/echo -e clear

	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=755
	+$(MAKE) -C Payload copy VERSION=755

	@/bin/echo -e "\e[1m\e[32m[7.55 Build Sucess!]\033[37m"

all:
	@/bin/echo -e clear

	+$(MAKE) -C ShellCode clean
	+$(MAKE) -C ShellCode

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=505
	+$(MAKE) -C Payload copy VERSION=505

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=672
	+$(MAKE) -C Payload copy VERSION=672

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=702
	+$(MAKE) -C Payload copy VERSION=702

	+$(MAKE) -C Payload clean
	+$(MAKE) -C Payload VERSION=755
	+$(MAKE) -C Payload copy VERSION=755

	@/bin/echo -e "\e[1m\e[32m[Build all Sucess!]\033[37m"
