#!/usr/bin/env ruby

require 'net/http'
require 'uri'

def download_file(url)
  uri = URI(url)
  http = Net::HTTP.new(uri.host, uri.port)
  http.use_ssl = true if uri.scheme == 'https'

  request = Net::HTTP::Get.new(uri)
  response = http.start { |http| http.request(request) }

  if response.code == '302'
    new_uri = URI(response['location'])
    return download_file(new_uri.to_s)
  elsif response.code == '200'
    return response.body
  else
    raise "Error downloading file: #{response.code} - #{response.message}"
  end
end

# Example usage:
url = 'https://github.com/takeiteasy/bla/raw/master/bla/bla.h'
content = download_file(url)
raw_lines = content.split("\n")
raw_lines[1] = raw_lines[1].gsub("bla", "fungl")
STATE_IN_COMMENT = 0
STATE_IN_HEADER = 1
STATE_IN_SOURCE = 2
state = 0
COMMENTS = []
HEADER = []
SOURCE = []
raw_lines.each do |line|
    case state
    when STATE_IN_COMMENT
        if line[0] == '#'
            HEADER.append line
            state = STATE_IN_HEADER
        else
            COMMENTS.append line
        end
    when STATE_IN_HEADER
        if line == "#ifdef BLA_IMPLEMENTATION"
            state = STATE_IN_SOURCE
        else
            HEADER.append line
        end
    else
        SOURCE.append line
    end
end

def fix_array(arr)
    arr.map { |a| a.gsub("BLA", "GLM").gsub("bla", "glm") }
end

comments = fix_array(COMMENTS)
header = comments + fix_array(HEADER)
source = comments + ['#include "glm.h"', ''] + fix_array(SOURCE)
source[0] = source[0].gsub("glm.h", "glm.c")

File.open("fungl/glm.h", "w") do |file|
    file.write(header.join("\n"))
end
File.open("fungl/glm.c", "w") do |file|
    source.pop
    file.write(source.join("\n"))
end
