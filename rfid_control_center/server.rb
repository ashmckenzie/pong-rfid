require 'sinatra'
require 'json'
require 'awesome_print'

post '/lookup' do
  content_type :json
  ap JSON.parse(request.body.read)
  { status: 'OK' }.to_json.to_s + "\n"
end
