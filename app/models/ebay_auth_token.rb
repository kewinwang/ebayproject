class EbayAuthToken < ActiveRecord::Base
  belongs_to :user

  # all the methods namespace rule:
  # if the methods top get the data from ebay ,use a prefix of "ebay"
  #  all "ebay" prefixed metheds return a ebay data typed object
  # like : ebay.get_session(...) return  a  Ebay::Responses::GetSessionID object
  #        my ebay prefixed methods  return a SessionID  object
  # this is get from the xml node if

  # to fetch token from ebay
  def ebayapi_get_fetch_token
    begin
      ebay=Ebay::Api.new
      if ebayapi_get_session_id && ebay_get_session_id.session_id
        # here maybe call by the view
        response = ebay.fetch_token(:session_id => ebayapi_get_session_id.session_id)
        if response.ack == "Success"
          auth_token = response.auth_token
          save_ebay_auth_token(auth_token)
        end
      end
    rescue Exception => e
      logger.info e.message
    end
  end

  #if exists update it ,else add a new
  def save_ebay_auth_token(*auth_token)
    begin
      @ebay_user = ebayapi_get_user
      @user_auth_token_exists = get_current_user_auth_token(@ebay_user.user_id)
      if @user_auth_token_exists
        # to update it
        ebay_auth_token=EbayAuthToken.find_by_user_id_and_ebay_uid(current_user.id,@ebay_user.user_id)
        ebay_auth_token.update_attribtes(:auth_token => auth_token.auth_token ,:expiration_time => auth_token.hard_expiration_time,:updated_at=> Time.now)

      elsif
        # to insert a new one
        EbayAuthToken.create(:token_set_name => "",:auth_token => auth_token.auth_token,:expiration_time => auth_token.hard_expiration_time, :user_id => current_user.id ,:primary_set => false, :ebay_uid => @ebay_user.user_id)
      end
    rescue Exception => e
      logger.info e.message

    end
  end

  # get ebay user
  def ebayapi_get_user
    ebay= Ebay::Api.new
    begin
      response = ebay.get_user
      if response.ack == "Success"
        ebayuser = reponse.user
      end
    rescue Exception => e
      logger.info e.message
    end
    ebayuser
  end

  # to get the ebay session_id
  def ebayapi_get_session_id
    ebay= Ebay::Api.new
    begin
      response = ebay.get_session_id(:ru_name => EBAY_RU_NAME)
      if response.ack =="Success"
        session_id = response.session_id
      end
    rescue  Exception => e
      logger.info e.message
    end
    session_id
  end

  # to get the current user ebay auth token from db
  def get_current_user_auth_token(user_id)
    user=User.find(user_id)
    if user.ebay_auth_token
      user.ebay_auth_token
    end
  end

  # remove a ebay auth token by user  id and ebay_uid
  def remove_ebay_auth_token(user_id,ebay_uid)
    begin
      ebay_token=EbayAuthToken.find_by_user_id_and_ebay_uid(user_id,ebay_uid)
      ebay_token.destroy
    rescue Exception => e
      loggr.info e.message
    end
  end

end
