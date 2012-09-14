#include <iostream>
#include <cstdio>
#include <bitset>
#include <sstream>
#include <list>
#include <iomanip>
#include <iterator>
#include <queue>
#include <cmath>
#include <map>
#include <set>
#include <list>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <stack>

#include <sys/prctl.h>
#include <sys/fcntl.h>
#include <unistd.h>

using namespace std;

const char* unsafeEnvironmentVariables[] = {
  "LD_AOUT_LIBRARY_PATH",
  "LD_AOUT_PRELOAD",
  "GCONV_PATH",
  "GETCONF_DIR",
  "HOSTALIASES",
  "LD_AUDIT",
  "LD_DEBUG",
  "LD_DEBUG_OUTPUT",
  "LD_DYNAMIC_WEAK",
  "LD_LIBRARY_PATH",
  "LD_ORIGIN_PATH",
  "LD_PRELOAD",
  "LD_PROFILE",
  "LD_SHOW_AUXV",
  "LD_USE_LOAD_BIAS",
  "LOCALDOMAIN",
  "LOCPATH",
  "MALLOC_TRACE",
  "NIS_PATH",
  "NLSPATH",
  "RESOLV_HOST_CONF",
  "RES_OPTIONS",
  "TMPDIR",
  "TZDIR"
};

#define ERR_QUIT(a,b) do { \
	if((b)) { \
		cout << a << endl; \
	} \
	exit(0); \
} while (0);

#define ERR_QUIT_FUNC(a,b) do { \
	if((b)) { \
		a; \
	} \
	exit(0); \
} while (0);

#define FOR_EACH_STRING(a,b) do { \
	for (int i = 0; i < sizeof(a)/sizeof(char*); ++i) \
	{ \
		b;\
	} \
} while (0);

void DropMyPrivileges(char **argv)
{
	gid_t rgid, egid, sgid;
	uid_t ruid, euid, suid;
	pid_t pid;

	ERR_QUIT( "Only root can do this !", getuid() == 0 );

	ERR_QUIT_FUNC( perror("prctl(PR_SET_DUMPABLE)"), prctl(PR_SET_DUMPABLE, 0, 0, 0, 0) );
	ERR_QUIT( "prctl(PR_GET_DUMPABLE) still works.", prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) );

	ERR_QUIT_FUNC( perror("getresgid"), getresgid(&rgid, &egid, &sgid) );
	ERR_QUIT_FUNC( perror("setresgid"), setresgid(rgid, rgid, rgid) );

	ERR_QUIT_FUNC( perror("getresuid"), getresuid(&ruid, &euid, &suid) );
	ERR_QUIT_FUNC( perror("setresuid"), setresuid(ruid, ruid, ruid) );

	// launch
	FOR_EACH_STRING(unsafeEnvironmentVariables,unsetenv(unsafeEnvironmentVariables[i]));

	ERR_QUIT_FUNC( perror("chdir"), chdir ("/proc/self/") );

	cout << "Argv[1]: " << argv[1] << endl;
	execv (argv[1], &argv[1]);
}

int main ( int argc , char **argv ) 
{
	DropMyPrivileges(argv);
	return 0;
}

