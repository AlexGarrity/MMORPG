/**     Alex Garrity, 2018      **/

#include "Engine.h"

// Constructor that I deleted was here.


bool Engine::Start ( short x, short y, short depth, const char name[] )
{
    WindowManager::CreateWindow ( x, y, depth, name );
    WindowManager::SetFramerate ( 0 );
    NetworkManager::InitialiseSockets ();
    ObjectManager::CreateEntity ( new Entity ( "000000000000000" ) );
    StateManager::Initialise();

    if ( WindowManager::WindowIsOpen() ) {
        /** TODO:  Change this to GameLoop() once the state manager works **/
        GameLoop();
    } else {
        return false;
    }

    return true;
}

/**
    Game Loop -
        Checks if the window is open
        If it is:
            Check that the user hasn't tried to close the window, and that it's still in focus
            Update the frame time
            Update the state manager
            Clear the window
            Display objects drawn to the window
        If not:
            End the loop (closes the application)
**/

void Engine::GraphicsUpdate()
{
    ObjectManager::DrawObjects();
    //EntityManager::DrawObject();       Maybe separate the two out?
}

void Engine::NetworkUpdate()
{
    while ( WindowManager::WindowIsOpen() ) {
        NetworkManager::Update();
    }
}

void Engine::GameLoop()
{
    std::thread networkThread ( &Engine::NetworkUpdate, this );     //Initialise the network manager thread, which runs separately from the game thread

    while ( WindowManager::WindowIsOpen() ) {
        Time::Update();
        WindowManager::CheckEvents();
        UIManager::Update();              //Make the UI manager check for inputs, using the event
        StateManager::Update();
        WindowManager::Clear();
        WindowManager::Display();
    }

    networkThread.detach();        //End network thread operations using detach, such that it's instant
}

/**
    This version of the game loop is purely for test purposes.
    When something new gets implemented, it gets tested here first.
    This is NOT the actual game loop.
        > This might yet turn into the actual gameloop...
    It's also a total mess...
**/

void Engine::TestLoop()
{
    ResourceManager::Load ( "assets/texture.png", "texture" );
    //ObjectManager::CreateObject("texture", 50, 50);

    //std::thread graphicsThread(Engine::GraphicsUpdate);           // Drawing and updating objects may be done on a separate thread
    std::thread networkThread ( &Engine::NetworkUpdate, this );   // Sending and receiving data to and from the server is done in a separate thread

    while ( WindowManager::WindowIsOpen() ) {
        WindowManager::CheckEvents();
        GraphicsUpdate();       //Updating graphics is on the main thread for now
    }

    //graphicsThread.join();
    networkThread.detach();     //Detach here as opposed to join as otherwise the network would have to wait for the next update
    NetworkManager::EndConnection();    //Tell the server that the connection has been closed, and free up the ports
}
