#include <SFML/Graphics.hpp>
#include <Motion/Motion.hpp>

int main()
{
    sf::RenderWindow window({ 640, 480 }, "Motion Playback Example"); // create the SFML window
    window.setVerticalSyncEnabled(true);

    // Create some buttons to control the playback
    sf::VertexArray playbutton(sf::PrimitiveType::Triangles);
    playbutton.append({ { 255, 350 }, sf::Color::Green });
    playbutton.append({ { 275, 360 }, sf::Color::Green });
    playbutton.append({ { 255, 370 }, sf::Color::Green });
    sf::VertexArray pausebutton(sf::PrimitiveType::Quads);
    pausebutton.append({ { 305, 350 }, sf::Color::Blue });
    pausebutton.append({ { 310, 350 }, sf::Color::Blue });
    pausebutton.append({ { 310, 370 }, sf::Color::Blue });
    pausebutton.append({ { 305, 370 }, sf::Color::Blue });
    pausebutton.append({ { 320, 350 }, sf::Color::Blue });
    pausebutton.append({ { 325, 350 }, sf::Color::Blue });
    pausebutton.append({ { 325, 370 }, sf::Color::Blue });
    pausebutton.append({ { 320, 370 }, sf::Color::Blue });
    sf::VertexArray stopbutton(sf::PrimitiveType::Quads);
    stopbutton.append({ { 355, 350 }, sf::Color::Red });
    stopbutton.append({ { 375, 350 }, sf::Color::Red });
    stopbutton.append({ { 375, 370 }, sf::Color::Red });
    stopbutton.append({ { 355, 370 }, sf::Color::Red });

    mt::DataSource datasource; // create the data source from which playback will happen
    if (!datasource.LoadFromFile("{video file path}")) // load a file into the data source
        return EXIT_FAILURE;

    mt::SFMLAudioPlayback audioplayback(datasource); // create an audio playback from our data source
    mt::SFMLVideoPlayback videoplayback(datasource); // create a video playback from our data source

    // scale video to fit the window
    videoplayback.setScale(640.f / static_cast<float>(datasource.GetVideoSize().x), 480.f / static_cast<float>(datasource.GetVideoSize().y));

    // standard SFML game and event loops
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (playbutton.getBounds().contains(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })))
                    datasource.Play(); // play button was pressed - so start the playback
                else if (pausebutton.getBounds().contains(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })))
                    datasource.Pause(); // pause button was pressed - so pause the playback
                else if (stopbutton.getBounds().contains(window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y })))
                    datasource.Stop(); // stop button was pressed - stop the playback
            }
        }

        datasource.Update(); // update the data source - this is required for any playbacks

        window.clear();
        window.draw(videoplayback); // draw the video playback
        // draw the buttons
        window.draw(playbutton);
        window.draw(pausebutton);
        window.draw(stopbutton);
        window.display();
    }
}
