#!/usr/bin/env ruby
# coding: utf-8
#Author: Roy L Zuo (roylzuo at gmail dot com)
#Description: 

module Jekyll
    module Filters
        def date_to_relative(date)
            date = Date.parse(date.to_s, true) unless date.is_a? Date
            days = (date - Date.today).to_i

            return 'today'     if days >= 0 and days < 1
            return 'tomorrow'  if days >= 1 and days < 2
            return 'yesterday' if days >= -1 and days < 0

            return "in #{days} days"      if days.abs < 60 and days > 0
            return "#{days.abs} days ago" if days.abs < 60 and days < 0

            return date.strftime('%A, %B %e') if days.abs < 182
            return date.strftime('%A, %B %e, %Y')
        end

        def list_array array
            array.collect{|i| i.to_s}.join(',')
        end
    end 
end
