#
# -*- py-indent-offset: 4; coding: iso-8859-1 -*-
#
# Copyright (C) 2008, 2009 Loic Dachary <loic@dachary.org>
#
# This software's license gives you freedom; you can copy, convey,
# propagate, redistribute and/or modify this program under the terms of
# the GNU Affero General Public License (AGPL) as published by the Free
# Software Foundation (FSF), either version 3 of the License, or (at your
# option) any later version of the AGPL published by the FSF.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
# General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program in a file in the toplevel directory called
# "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
#

# borrowed from memcache.py
import types
import memcache

class MemcachedStringEncodingError(Exception):
    pass

def check_key(key, key_extra_len=0):
    """Checks sanity of key.  Fails if:
        Key length is > SERVER_MAX_KEY_LENGTH (Raises MemcachedKeyLength).
        Contains control characters  (Raises MemcachedKeyCharacterError).
        Is not a string (Raises MemcachedStringEncodingError)
    """
    if type(key) == types.TupleType: key = key[1]
    if not isinstance(key, str):
        raise MemcachedStringEncodingError, ("Keys must be str()'s, not"
                "unicode.  Convert your unicode strings using "
                "mystring.encode(charset)!")

memcache_singleton = {}
memcache_log_singleton = []
memcache_expiration_singleton = {}
class _MemcacheMockupHost:
    def __init__(self,address):
        host,port = address.split(':')
        self.address = (host,int(port))
    def __str__(self):
        return '%s:%d' % self.address
    
class MemcacheMockup:
    class Client:
        def __init__(self, servers, *args, **kwargs):
            self.servers = [_MemcacheMockupHost(i) for i in servers]
            self.cache = memcache_singleton
            self.expiration = memcache_expiration_singleton
            self.log = memcache_log_singleton

        def get(self, key):
            check_key(key)
            if key in self.cache:
                return self.cache[key]
            else:
                return None

        def get_multi(self, keys):
            r = {}
            for key in keys:
                if key in self.cache:
                    r[key] = self.cache[key]
            return r
        
        def set(self, key, value, time = 0):
            check_key(key)
            self.cache[key] = value
            self.expiration[key] = time
            self.log.append(('set', (key, value, time)))

        def set_multi(self, kwargs, time = 0):
            self.cache.update(kwargs)
            for k in kwargs: self.expiration[k] = time
            return []

        def add(self, key, value, time = 0):
            if key in self.cache:
                return 0
            else:
                self.cache[key] = value
                self.expiration[key] = time                
                return 1

        def replace(self, key, value, time = 0):
            if key in self.cache:
                self.cache[key] = value
                self.expiration[key] = time
                return 1
            else:
                return 0
            
        def delete(self, key):
            check_key(key)
            try:
                del self.cache[key]
                return 1
            except:
                return 0

        def delete_multi(self, keys):
            for key in keys:
                if key in self.cache:
                    del self.cache[key]
            return 1


def checkMemcacheServers(memcache_client):
    '''
    checks if all memcached servers are actually online.
    throws an error if the connect does not work on one of
    '''
    import socket
    servers_offline = []
    for address in [x.address for x in memcache_client.servers]:
        try:
            s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            s.settimeout(0.5)
            s.connect(address)
            s.shutdown(socket.SHUT_RDWR)
            s.close()
        except Exception, e:
            servers_offline.append((address,e))
            continue
    
    if servers_offline:
        raise Exception('Memcached connectity errors: %s' % str(servers_offline))
