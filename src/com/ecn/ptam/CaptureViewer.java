package com.ecn.ptam;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.media.MediaPlayer;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.widget.Button;

/*
 * Manages the state of the application, calls PTAM to update its state.
 * Sends KeyPress events to PTAM to simulate a user interaction.
 * Manages the Logger to save information on the disk.
 * Emits the beep sound to synchronize the video with the logged positions.
 */
public class CaptureViewer extends GLSurfaceView implements
		View.OnClickListener {

	private BatchRenderer _renderer;
	private Logger _logger;
	private MediaPlayer _player;

	private enum State {
		INIT, STEREO, ZONE, STANDBY, TRACKING
	};

	private State _state;
	private Button _button;
	Map<State, String> _buttonmap;

	public CaptureViewer(Context context, VideoSource videosource) {
		super(context);

		_state = State.INIT;
		_logger = new Logger(context, "positions.log");
		load_beep(context);

		CaptureRenderer capture = new CaptureRenderer(videosource, this);
		CameraRenderer camera = new CameraRenderer(videosource);
		_renderer = new BatchRenderer();
		_renderer.add(camera);
		_renderer.add(capture);
		setRenderer(_renderer);

		_buttonmap = new HashMap<State, String>();
		_buttonmap.put(State.INIT, "Start Initialization");
		_buttonmap.put(State.STEREO, "End Initialization");
		_buttonmap.put(State.ZONE, "Save Keypoint");
		_buttonmap.put(State.STANDBY, "Start");
		_buttonmap.put(State.TRACKING, "Stop");
	}

	public void set_action_button(Button btn) {
		_button = btn;
	}

	public void load_beep(Context ctx) {
		_player = MediaPlayer.create(ctx, R.raw.beep);
		try {
			_player.prepare();
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void update() {
		if (_state == State.TRACKING) {
			_logger.write(PTAM.getModelView());
		}
	}

	private void change_state(State dest) {
		_state = dest;
		_button.setText(_buttonmap.get(dest));
	}

	public void reset_all() {
		PTAM.send("r");
		change_state(State.INIT);
	}

	public void reset_zone() {
		PTAM.send("c");
		change_state(State.ZONE);
	}

	public void reset_tracking() {
		PTAM.send("t");
		change_state(State.STANDBY);
	}

	private void init_handler() {
		PTAM.send("Space");
		change_state(State.STEREO);
	}

	private void stereo_handler() {
		PTAM.send("Space");
		change_state(State.ZONE);
	}

	private void zone_handler() {
		PTAM.send("Enter");
		if (PTAM.objectIsGood()) {
			_logger.write_mat(PTAM.getCorners());
			change_state(State.STANDBY);
		}
	}

	public void standby_handler() {
		_player.start();
		_logger.log_beep();
		PTAM.start();
		change_state(State.TRACKING);
	}

	public void tracking_handler() {
		PTAM.stop();
		_logger.flush();
		change_state(State.STANDBY);
	}

	@Override
	public void onClick(View v) {
		switch (_state) {
		case INIT:
			init_handler();
			break;
		case STEREO:
			stereo_handler();
			break;
		case ZONE:
			zone_handler();
			break;
		case STANDBY:
			standby_handler();
			break;
		case TRACKING:
			tracking_handler();
			break;
		default:
			break;
		}
	}

	public void onPause() {
		_logger.flush();
		_logger.close();
	}
}
