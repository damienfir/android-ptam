package com.ecn.ptam;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;

import android.content.Context;
import android.os.Environment;
import android.util.Log;


/*
 * Creates the log file and provides an interface to write positions, matrices and timestamp into it
 */
public class Logger {
	
	private BufferedWriter _buf;
	private Context _ctx;
	private String _filename;
	private float[] _corners;
	
	public Logger(Context context, String filename) {
		_ctx = context;
		_filename = filename;
		reset();
	}
	
	public void reset() {
		_buf = new BufferedWriter(create_file(_filename));
	}
	
	public void reset_positions() {
		reset();
		write_corners(_corners);
	}
	
	public FileWriter create_file(String filename) {
		File file = new File(_ctx.getExternalFilesDir(null), filename);
		if (file != null) {
			file.delete();
		}
//		file.mkdirs();
		FileWriter writer = null;
		try {
			writer = new FileWriter(file);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return writer;
	}
	
	public void log_beep(long t) {
		float[] mat = new float[16];
		Arrays.fill(mat, 0);
		write(t, mat, true);
	}
	
	public void write(long time, float mv[], boolean is_good) {
		Log.i("PTAM", ""+time);
		String str = "" + time;
		for (float f : mv) {
			str += " " + f ;
		}
		str += " " + (is_good?1.f:0.f);
		write_line(str);
	}
	
	public void write_corners(float mat[]) {
		_corners = mat;
		
		for (int i = 0; i < 3; ++i) {
			String str = " " + 0;
			for (int j = 0; j < 16; ++j) {
				str += " " + mat[i*16+j];
			}
			write_line(str);
		}
		flush();
	}
	
	public void write_line(String str) {
		try {
			_buf.write(str);
			_buf.newLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void flush() {
		try {
			_buf.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public void close() {
		try {
			_buf.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/* Checks if external storage is available for read and write */
	public boolean isExternalStorageWritable() {
	    String state = Environment.getExternalStorageState();
	    if (Environment.MEDIA_MOUNTED.equals(state)) {
	        return true;
	    }
	    return false;
	}
	
	/* Checks if external storage is available to at least read */
	public boolean isExternalStorageReadable() {
	    String state = Environment.getExternalStorageState();
	    if (Environment.MEDIA_MOUNTED.equals(state) ||
	        Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
	        return true;
	    }
	    return false;
	}
}