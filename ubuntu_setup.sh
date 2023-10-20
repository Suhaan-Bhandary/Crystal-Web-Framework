#!/bin/bash

function installDependencies {
    apt update
    apt install build-essential
    apt install make
    apt install jq
    apt install inotify-tools
}

function runBuildServer {
    # Clean bin
    make clean

    # Build the project
    make all

    # Run the build
    ./build/bin/httpScratchServer
}

function runDevelopmentServer {
    ./monitor/monitor.sh
}

# Menu for running the above functions
PS3="Select the Task to run: "

select commandToRun in "Install Dependencies" "Run Development Server" "Build Server" Exit; do
    case $commandToRun in
    "Install Dependencies")
        installDependencies
        break
        ;;
    "Run Development Server")
        runDevelopmentServer
        break
        ;;
    "Build Server")
        runBuildServer
        break
        ;;
    "Exit")
        echo "Exiting the Script."
        break
        ;;
    *)
        echo "Ooops"
        ;;
    esac
done
