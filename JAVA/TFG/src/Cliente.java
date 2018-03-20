import java.io.DataOutputStream;
import java.io.IOException;

public class Cliente extends Conexion
{
	public Cliente() throws IOException{super("cliente");} //Se usa el constructor para cliente de Conexion

	public void startClient() //Metodo para iniciar el cliente
	{
		try
		{
			//Flujo de datos hacia el servidor
			salidaServidor = new DataOutputStream(cs.getOutputStream());

			//Se enviaran dos mensajes
			for (int i = 0; i < 2; i++)
			{
				//Se escribe en el servidor usando su flujo de datos
				salidaServidor.writeUTF("Este es el mensaje numero " + (i+1) + "\n");
			}

			cs.close();//Fin de la conexion

		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}
	}
}
