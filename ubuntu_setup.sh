#!/bin/bash

function installDependencies {
    apt update
    apt install sqlite3
    apt-get install libsqlite3-dev
    apt install build-essential
    apt install make
    apt install jq
    apt install inotify-tools
    apt install cmake
}

function runBuildServer {
    ./scripts/deploy_example.sh
}

function runDevelopmentServer {
    ./monitor/monitor.sh
}

function runCleanDevelopmentServer {
    ./monitor/monitor.sh -c
}

# Menu for running the above functions
PS3="Select the Task to run: "

select commandToRun in "Install Dependencies" "Run Development Server" "Run Clean Development Server" "Build Server" Exit; do
    case $commandToRun in
    "Install Dependencies")
        installDependencies
        break
        ;;
    "Run Development Server")
        runDevelopmentServer
        break
        ;;
    "Run Clean Development Server")
        runCleanDevelopmentServer
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
