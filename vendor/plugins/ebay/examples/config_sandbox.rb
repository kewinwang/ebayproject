# Copy this file and paste it into a new file called config.rb
# Then, replace the values where appropriate below and save.
AUTH_TOKEN="AgAAAA**AQAAAA**aAAAAA**z7JMTg**nY+sHZ2PrBmdj6wVnY+sEZ2PrA2dj6wFk4CoAZiApw6dj6x9nY+seQ**rJsBAA**AAMAAA**PPVX9UIFK+PrlYboUZnb2bwBcCPl/PJhkt4XbwRHNkkagLPDlKQAlx+/dalZGWA4Fa30kadALx9UOUFO3H1eKbacAlOkHlrnVtRdyD9xOashivdH2NfykjzkevV8ju+ua13qfl3Ent1rklnJM4NFWJOiXvavjA7kDGyQ6xTUDQwL8YR4+zoAhaQBvwK33Q++m9i/VfA2X5UfPOLCtPDP1YIx60I10n8lktrMGV4Kqe8+fiXk32nmr15Dpx80ImRshn7U6k4NVGxn3jnHd1IHIvVpAGMGbMWMcKtpmBWejT0X/2NPPKZd2fXIqsiNTNRBHGohh/7rtAbU+9oncuLn7QTWrlNd3YgmEUFEJJLBSxIfexFeSzjPlZ68ELkilf8J174Qoq1kR1yGE0az4b7foCC3EdsflSbAuQcCDOILHb9XbArTY4vxRbe6GLvnyaob2ZJAABkm70yi96SEmAI4M3a5G6bDzO8Vw8/6taguom1jsqZOQYv4WVpEuLwSkgWgYEV3kANm970HHjRvlbP5SXufmREEKUt9jYUmdTWsM4hK9Uv2rVUYr0mhqyQX8/7j08bCAsLSLL0UZiX6I8PtNyu9K2RMTCwpZspJn8+iCFEhOMnZp8yybkt2GeE9L+Uok5SAWARgL/YQyv7XRdtQtIoT9iJIug5DKnNnmCc9iyS3hiqn03CdNJh7VJKtVHRNbHdiELmkoEtYW51sqfLliRWAuindDeQKV+dsRJweIXDU7GJWyxxvF9FX21i4VkmA"
$:.unshift File.dirname(__FILE__)
$:.unshift File.join(File.dirname(__FILE__),'..', 'lib')
Ebay::Api.configure do |ebay|
  ebay.auth_token = AUTH_TOKEN
  ebay.dev_id = '4227fe7e-7973-43c3-99c9-eb400603d0cb'
  ebay.app_id = 'Kewin1d89-0043-47ac-a6cf-ed86488052b'
  ebay.cert = '299a226b-4acf-4797-85a7-16b759cc91bc'

  # The default environment is the production environment
  # Override by setting use_sandbox to true
  ebay.use_sandbox = true
  
  # If you need to run any of the following:
  # SetReturnURL GetReturnURL GetRuName FetchToken
  # Then you'll need to set the username and password for the account
  ebay.username = 'testuser_kewin'
  ebay.password = '@Keiwn384677'
end
