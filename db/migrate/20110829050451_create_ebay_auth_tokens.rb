class CreateEbayAuthTokens < ActiveRecord::Migration
  def self.up
    create_table :ebay_auth_tokens do |t|
      t.string :token_set_name     ,:default => ""
      t.text :auth_token           ,:null => false
      t.datetime :expiration_time
      t.integer :user_id           ,:null => false
      t.boolean :primary_set       ,:default => false
      t.string :ebay_uid           ,:null => false

      t.timestamps
    end
  end

  def self.down
    drop_table :ebay_auth_tokens
  end
end
