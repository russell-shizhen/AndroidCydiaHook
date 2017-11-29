package com.example.shizzhan.sampleapp;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        // The target .so file to be hooked.
        System.loadLibrary("fooSo");
        // The hooking module, note that the .cy is necessary for CydiaSubstrate to recognize it correctly.
        System.loadLibrary("hook.cy");
        // CydiaSubstrate dependencies.
        System.loadLibrary("substrate");
        System.loadLibrary("substrate-dvm");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button testPassword = findViewById(R.id.test_password);
        testPassword.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String resultString = testPassword("faked_password") == 0 ? "password success":"password failed";
                Toast.makeText(MainActivity.this, resultString, Toast.LENGTH_LONG).show();
            }
        });

        final Button testStrncmp = findViewById(R.id.test_strncmp);
        testStrncmp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String resultString = testStrncmp("apple", "banana", 5) == 0 ? "strncmp success":"strncmp failed";
                Toast.makeText(MainActivity.this, resultString, Toast.LENGTH_LONG).show();
            }
        });

    }

    public native int testPassword(String password);
    public native int testStrncmp(String s1, String s2, int size);

}
