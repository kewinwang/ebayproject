# Copy this file and paste it into a new file called config.rb
# Then, replace the values where appropriate below and save.
AUTH_TOKEN="AgAAAA**AQAAAA**aAAAAA**BmZTTg**nY+sHZ2PrBmdj6wVnY+sEZ2PrA2dj6wFk4CoAZiApw6dj6x9nY+seQ**rJsBAA**AAMAAA**PPVX9UIFK+PrlYboUZnb2bwBcCPl/PJhkt4XbwRHNkkagLPDlKQAlx+/dalZGWA4Fa30kadALx9UOUFO3H1eKbacAlOkHlrnVtRdyD9xOashivdH2NfykjzkevV8ju+ua13qfl3Ent1rklnJM4NFWJOiXvavjA7kDGyQ6xTUDQwL8YR4+zoAhaQBvwK33Q++m9i/VfA2X5UfPOLCtPDP1YIx60I10n8lktrMGV4Kqe8+fiXk32nmr15Dpx80ImRshn7U6k4NVGxn3jnHd1IHIvVpAGMGbMWMcKtpmBWejT0X/2NPPKZd2fXIqsiNTNRBHGohh/7rtAbU+9oncuLn7QTWrlNd3YgmEUFEJJLBSxIfexFeSzjPlZ68ELkilf8J174Qoq1kR1yGE0az4b7foCC3EdsflSbAuQcCDOILHb9XbArTY4vxRbe6GLvnyaob2ZJAABkm70yi96SEmAI4M3a5G6bDzO8Vw8/6taguom1jsqZOQYv4WVpEuLwSkgWgYEV3kANm970HHjRvlbP5SXufmREEKUt9jYUmdTWsM4hK9Uv2rVUYr0mhqyQX8/7j08bCAsLSLL0UZiX6I8PtNyu9K2RMTCwpZspJn8+iCFEhOMnZp8yybkt2GeE9L+Uok5SAWARgL/YQyv7XRdtQtIoT9iJIug5DKnNnmCc9iyS3hiqn03CdNJh7VJKtVHRNbHdiELmkoEtYW51sqfLliRWAuindDeQKV+dsRJweIXDU7GJWyxxvF9FX21i4VkmA"
$:.unshift File.dirname(__FILE__)
$:.unshift File.join(File.dirname(__FILE__),'..', 'lib')
Ebay::Api.configure do |ebay|
  ebay.auth_token = AUTH_TOKEN
  ebay.dev_id = '7710cb84-954c-4c44-9941-aeffa9ebb12a'
  ebay.app_id = 'tuanpinc-d05b-46d5-9233-2e4e8451ab78'
  ebay.cert = '8571f145-ce75-44ab-9636-1b231133ad69'

  # The default environment is the production environment
  # Override by setting use_sandbox to true
  ebay.use_sandbox = false
  
  # If you need to run any of the following:
  # SetReturnURL GetReturnURL GetRuName FetchToken
  # Then you'll need to set the username and password for the account
  ebay.username = 'kewin'
  ebay.password = '@Keiwn384677'
end
