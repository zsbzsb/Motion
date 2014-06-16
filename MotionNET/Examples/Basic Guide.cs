using System;
using SFML.System;
using SFML.Window;
using SFML.Graphics;
using MotionNET;

namespace MotionNET_Example
{
    class Basic_Guide
    {
        static void Main(string[] args)
        {
            RenderWindow window = new RenderWindow(new VideoMode(640, 480), "MotionNET Playback Example");
            window.SetVerticalSyncEnabled(true);

            // Create some buttons to control the playback
            VertexArray playbutton = new VertexArray(PrimitiveType.Triangles, 3);
            playbutton[0] = new Vertex(new Vector2f(255, 350), Color.Green);
            playbutton[1] = new Vertex(new Vector2f(275, 360), Color.Green);
            playbutton[2] = new Vertex(new Vector2f(255, 370), Color.Green);
            VertexArray pausebutton = new VertexArray(PrimitiveType.Quads, 8);
            pausebutton[0] = new Vertex(new Vector2f(305, 350), Color.Blue);
            pausebutton[1] = new Vertex(new Vector2f(310, 350), Color.Blue);
            pausebutton[2] = new Vertex(new Vector2f(310, 370), Color.Blue);
            pausebutton[3] = new Vertex(new Vector2f(305, 370), Color.Blue);
            pausebutton[4] = new Vertex(new Vector2f(320, 350), Color.Blue);
            pausebutton[5] = new Vertex(new Vector2f(325, 350), Color.Blue);
            pausebutton[6] = new Vertex(new Vector2f(325, 370), Color.Blue);
            pausebutton[7] = new Vertex(new Vector2f(320, 370), Color.Blue);
            VertexArray stopbutton = new VertexArray(PrimitiveType.Quads, 4);
            stopbutton[0] = new Vertex(new Vector2f(355, 350), Color.Red);
            stopbutton[1] = new Vertex(new Vector2f(375, 350), Color.Red);
            stopbutton[2] = new Vertex(new Vector2f(375, 370), Color.Red);
            stopbutton[3] = new Vertex(new Vector2f(355, 370), Color.Red);

            DataSource datasource = new DataSource(); // create the data source from which playback will happen
            if (!datasource.LoadFromFile("{video file path}")) // load a file into the data source
                return;

            AudioPlayback audioplayback = new AudioPlayback(datasource); // create an audio playback from our data source
            VideoPlayback videoplayback = new VideoPlayback(datasource); // create a video playback from our data source

            // scale video to fit the window
            videoplayback.Scale = new Vector2f(640f / (float)datasource.VideoSize.X, 480f / (float)datasource.VideoSize.Y);

            // standard SFML.NET game and event handling
            window.Closed += (sender, e) => { window.Close(); };
            window.MouseButtonPressed += (sender, e) =>
                {
                    Vector2f mousepos = window.MapPixelToCoords(new Vector2i(e.X, e.Y));
                    if (playbutton.Bounds.Contains(mousepos.X, mousepos.Y))
                        datasource.Play();
                    else if (pausebutton.Bounds.Contains(mousepos.X, mousepos.Y))
                        datasource.Pause();
                    else if (stopbutton.Bounds.Contains(mousepos.X, mousepos.Y))
                        datasource.Stop();
                };
            while (window.IsOpen)
            {
                window.DispatchEvents();

                datasource.Update(); // update the data source - this is required for any playbacks

                window.Clear();
                window.Draw(videoplayback); // draw the video playback
                // draw the buttons
                window.Draw(playbutton);
                window.Draw(pausebutton);
                window.Draw(stopbutton);
                window.Display();
            }
        }
    }
}
