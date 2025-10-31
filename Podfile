# Uncomment the next line to define a global platform for your project
platform :ios, '16.0'

install! 'cocoapods', :disable_input_output_paths => true

source 'https://cdn.cocoapods.org/' 
 #this is the comelit podspec sources
source 'https://bitbucket.org/comelit/public-comelit-pods.git'


use_frameworks! 

target 'HBuilder' do # <- your app target name
#    project '/Users/david/Desktop/uniapp/SDK/HBuilder-ExampleDemo/HBuilder-Hello.xcodeproj'  #relative path to your app xcode project

    #your application pods here

    #add this line to install the CGModule
    pod 'CGModule', '0.7.4'
end


#due to a cocoapod limitation (https://github.com/CocoaPods/CocoaPods/issues/9232)
#you need to force this flag in the post_install hook
post_install do |pi|
    pi.pods_project.targets.each do |t|
        t.build_configurations.each do |config|
            config.build_settings['BUILD_LIBRARY_FOR_DISTRIBUTION'] = 'YES'
            config.build_settings['IPHONEOS_DEPLOYMENT_TARGET'] = '16.0'
            config.build_settings['ENABLE_BITCODE'] = 'NO'
        end
    end
end

