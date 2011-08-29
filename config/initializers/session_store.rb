# Be sure to restart your server when you modify this file.

# Your secret key for verifying cookie session data integrity.
# If you change this key, all old sessions will become invalid!
# Make sure the secret is at least 30 characters and all random, 
# no regular words or you'll be exposed to dictionary attacks.
ActionController::Base.session = {
  :key         => '_reseller_session',
  :secret      => 'cfdaf92ac01c25a84f069dfbc7c86de9b0a814ef5bb27c688692dfd99cfebdc5bfd4733d7474f9e570d2f0876bbb863545b3918729f4957c761240cb11cb3885'
}

# Use the database for sessions instead of the cookie-based default,
# which shouldn't be used to store highly confidential information
# (create the session table with "rake db:sessions:create")
# ActionController::Base.session_store = :active_record_store
