import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

public class Servidor extends Conexion //Se hereda de conexion para hacer uso de los sockets y demas
{
    public Servidor() throws IOException{super("servidor");} //Se usa el constructor para servidor de Conexion

    public void startServer()//Metodo para iniciar el servidor
    {
        try
        {
            System.out.println("Esperando..."); //Esperando conexion

            cs = ss.accept(); //Accept comienza el socket y espera una conexion desde un cliente

            System.out.println("Cliente en linea");

            //Se obtiene el flujo de salida del cliente para enviarle mensajes
            salidaCliente = new DataOutputStream(cs.getOutputStream());

            //Se le envia un mensaje al cliente usando su flujo de salida
            salidaCliente.writeUTF("Peticion recibida y aceptada");

            //Se obtiene el flujo entrante desde el cliente
            BufferedReader entrada = new BufferedReader(new InputStreamReader(cs.getInputStream()));

            while((mensajeServidor = entrada.readLine()) != null) //Mientras haya mensajes desde el cliente
            {
                //Se muestra por pantalla el mensaje recibido
                System.out.println(mensajeServidor);
            }

            System.out.println("Fin de la conexion");

            ss.close();//Se finaliza la conexion con el cliente
        }
        catch (Exception e)
        {
            System.out.println(e.getMessage());
        }
    }
}