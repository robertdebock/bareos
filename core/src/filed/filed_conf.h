/*
   BAREOS® - Backup Archiving REcovery Open Sourced

   Copyright (C) 2000-2007 Free Software Foundation Europe e.V.
   Copyright (C) 2011-2012 Planets Communications B.V.
   Copyright (C) 2013-2016 Bareos GmbH & Co. KG

   This program is Free Software; you can redistribute it and/or
   modify it under the terms of version three of the GNU Affero General Public
   License as published by the Free Software Foundation and included
   in the file LICENSE.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.
*/
/*
 * Bareos File Daemon specific configuration
 *
 * Kern Sibbald, Sep MM
 */

#ifndef BAREOS_FILED_FILED_CONF_H_
#define BAREOS_FILED_FILED_CONF_H_ 1

#include "lib/messages_resource.h"
#include "lib/tls_conf.h"

class alist;
class dlist;

namespace filedaemon {

static const std::string default_config_filename("bareos-fd.conf");

/*
 * Resource codes -- they must be sequential for indexing
 */
enum
{
  R_DIRECTOR = 1001,
  R_CLIENT,
  R_MSGS,
  R_STORAGE,
  R_JOB,
  R_FIRST = R_DIRECTOR,
  R_LAST = R_JOB /* keep this updated */
};

/*
 * Some resource attributes
 */
enum
{
  R_NAME = 1020,
  R_ADDRESS,
  R_PASSWORD,
  R_TYPE
};

/* Definition of the contents of each Resource */
class DirectorResource
    : public BareosResource
    , public TlsResource {
 public:
  char* address = nullptr;          /* Director address or zero */
  uint32_t port = 0;                /* Director port */
  bool conn_from_dir_to_fd = false; /* Allow incoming connections */
  bool conn_from_fd_to_dir = false; /* Connect to director */
  bool monitor; /* Have only access to status and .status functions */
  alist* allowed_script_dirs =
      nullptr; /* Only allow to run scripts in this directories */
  alist* allowed_job_cmds = nullptr; /* Only allow the following Job commands to
                              be executed */
  uint64_t max_bandwidth_per_job = 0; /* Bandwidth limitation (per director) */

  DirectorResource() = default;
  virtual ~DirectorResource() = default;

  void ShallowCopyTo(BareosResource* p) const override
  {
    DirectorResource* r = dynamic_cast<DirectorResource*>(p);
    if (r) { *r = *this; }
  }
};

class ClientResource
    : public BareosResource
    , public TlsResource {
 public:
  ClientResource() = default;
  virtual ~ClientResource() = default;

  void ShallowCopyTo(BareosResource* p) const override
  {
    ClientResource* r = dynamic_cast<ClientResource*>(p);
    if (r) { *r = *this; }
  }

  dlist* FDaddrs = nullptr;
  dlist* FDsrc_addr = nullptr; /* Address to source connections from */
  char* working_directory = nullptr;
  char* pid_directory = nullptr;
  char* subsys_directory = nullptr;
  char* plugin_directory = nullptr; /* Plugin directory */
  alist* plugin_names = nullptr;
  char* scripts_directory = nullptr;
  MessagesResource* messages = nullptr; /* Daemon message handler */
  uint32_t MaxConcurrentJobs = 0;
  uint32_t MaxConnections = 0;
  utime_t SDConnectTimeout = {0};       /* Timeout in seconds */
  utime_t heartbeat_interval = {0};     /* Interval to send heartbeats */
  uint32_t max_network_buffer_size = 0; /* Max network buf size */
  uint32_t jcr_watchdog_time = 0;       /* Absolute time after which a Job gets
                                       terminated       regardless of its progress */
  bool compatible = false;              /* Support old protocol keywords */
  bool allow_bw_bursting = false; /* Allow bursting with bandwidth limiting */
  bool pki_sign =
      false; /* Enable Data Integrity Verification via Digital Signatures */
  bool pki_encrypt = false;                        /* Enable Data Encryption */
  char* pki_keypair_file = nullptr;                /* PKI Key Pair File */
  alist* pki_signing_key_files = nullptr;          /* PKI Signing Key Files */
  alist* pki_master_key_files = nullptr;           /* PKI Master Key Files */
  crypto_cipher_t pki_cipher = CRYPTO_CIPHER_NONE; /* PKI Cipher to use */
  bool nokeepalive = false;     /* Don't use SO_KEEPALIVE on sockets */
  bool always_use_lmdb = false; /* Use LMDB for accurate data */
  uint32_t lmdb_threshold = 0;  /* Switch to using LDMD when number of accurate
                               entries exceeds treshold. */
  X509_KEYPAIR* pki_keypair = nullptr; /* Shared PKI Public/Private Keypair */
  alist* pki_signers = nullptr;        /* Shared PKI Trusted Signers */
  alist* pki_recipients = nullptr;     /* Shared PKI Recipients */
  alist* allowed_script_dirs =
      nullptr; /* Only allow to run scripts in this directories */
  alist* allowed_job_cmds = nullptr; /* Only allow the following Job commands to
                                be executed */
  char* verid = nullptr;             /* Custom Id to print in version command */
  char* secure_erase_cmdline = nullptr; /* Cmdline to execute to perform secure
                                  erase of file */
  char* log_timestamp_format = nullptr; /* Timestamp format to use in generic
                                 logging messages */
  uint64_t max_bandwidth_per_job = 0;   /* Bandwidth limitation (global) */
};


ConfigurationParser* InitFdConfig(const char* configfile, int exit_code);
bool PrintConfigSchemaJson(PoolMem& buffer);

} /* namespace filedaemon */
#endif /* BAREOS_FILED_FILED_CONF_H_ */
