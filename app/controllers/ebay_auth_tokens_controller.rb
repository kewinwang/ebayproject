class EbayAuthTokensController < ApplicationController 
   def show
     @user= current_user
     current_user_tokens =  @user.ebay_auth_tokens
   end
   
   def new
   
   end
   
   
   
   def create
   
   end
   def show
   
   end 
end
