import serial
import time


def test_serial():
    ledState = 0
    try:
        # Open connection
        ser = serial.Serial('COM10', 115200, timeout=2)
        print(f"Connected to {ser.name}")

        while 1:
            # Wait for device initialization
            time.sleep(1)
            
            ledState = 1 ^ ledState

            # Send test message
            test_msg = '{"led":' + str(ledState) + '}'
            ser.write(test_msg.encode())
            print(f"Sent: {test_msg.strip()}")
            
            # Read response
            response = ser.readline()
            print(f"Received: {response.decode('utf-8').strip()}")
            
        
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except FileNotFoundError:
        print("Port not found - check device connection")
    except PermissionError:
        print("Permission denied - see troubleshooting guide")
    finally:
        # Close connection
        ser.close()
        print("Connection closed")

if __name__ == "__main__":
    test_serial()