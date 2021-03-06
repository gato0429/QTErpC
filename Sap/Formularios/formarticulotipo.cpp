#include "formarticulotipo.h"
#include "ui_formarticulotipo.h"

#include <QFileDialog>

FormArticuloTipo::FormArticuloTipo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormArticuloTipo)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window
                         | Qt::FramelessWindowHint
                         | Qt::WindowMinimizeButtonHint
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowCloseButtonHint);

     QPixmap bkgnd(FondoForm);
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Background, bkgnd);
        this->setPalette(palette);

    ui->guardar->setIcon(QIcon(BotonGuardar));
    ui->modificar->setIcon(QIcon(BotonModificar));
    ui->eliminar->setIcon(QIcon(BotonEliminar));

    Estado=INSERTAR;
    ui->modificar->setEnabled(false);
    ui->eliminar->setEnabled(false);
    ui->guardar->setEnabled(true);

    mRepisa=RepisaArticuloTipo::Iniciar();
}

void FormArticuloTipo::SetArticuloTipo(ArticuloTipo valor)
{

    ui->nombre->setText(valor.getNombre());
    ui->codigo->setText(valor.getCodigo());

    QPixmap pix;
    pix.load(RutaImagenes+valor.getImagen());
    ui->imagenlabel->setPixmap(pix.scaled(60,60,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    ui->guardar->setEnabled(false);
    ui->modificar->setEnabled(true);
    ui->eliminar->setEnabled(true);
    ui->nombre->setEnabled(false);
    ui->BotonArchivo->setEnabled(false);
}

FormArticuloTipo::~FormArticuloTipo()
{
    delete ui;
}

void FormArticuloTipo::on_BotonArchivo_clicked()
{
    VisorImagenes* v=new VisorImagenes(this);
    v->exec();

}

void FormArticuloTipo::on_tabWidget_tabBarClicked(int index)
{
    if(index==1)
    {
    this->close();
    }
}

void FormArticuloTipo::on_guardar_clicked()
{
    FabricaBaseDatos* Bd=FabricaBaseDatos::IniciarFabrica(POSTGRES);
    FabricaArticuloTipos* Fobj=Bd->Fabrica->CrearArticuloTipo();

    if(Estado==INSERTAR)
    {
        ArticuloTipo* objeto=new ArticuloTipo();
        objeto->setImagen(ui->imagen->text());
        objeto->setNombre(ui->nombre->text());



        if(Bd->Fabrica->Conectar())
        {

            if(Fobj->Insertar(*objeto))
            {
                QMessageBox mensaje;
                mensaje.setText("El Tipo de Articulo fue Añadido a la Base de Datos");
                mensaje.setIcon(QMessageBox::Information);
                mensaje.exec();
            }
            else
            {
                QMessageBox mensaje;
                mensaje.setText("Error el articulo no fue añadido");
                mensaje.setIcon(QMessageBox::Warning);
                mensaje.exec();
            }
        }
        else
        {
            QMessageBox mensaje;
            mensaje.setText("Error no hay conexion con la base de datos");
            mensaje.setIcon(QMessageBox::Warning);
            mensaje.exec();

        }
    }

    if(Estado==MODIFICAR)
    {
        ArticuloTipo* objAnt=new ArticuloTipo();
        objAnt->setCodigo(ui->codigo->text());

        ArticuloTipo* objMod=new ArticuloTipo();
        objMod->setImagen(ui->imagen->text());
        objMod->setNombre(ui->nombre->text());

        if(Bd->Fabrica->Conectar())
        {
            if(Fobj->Actualizar(*objAnt,*objMod))
            {
                QMessageBox mensaje;
                mensaje.setText("El Tipo de Articulo fue Modificado con Exito");
                mensaje.setIcon(QMessageBox::Information);
                mensaje.exec();
            }
            else
            {
                QMessageBox mensaje;
                mensaje.setText("Error el articulo no fue Modificado");
                mensaje.setIcon(QMessageBox::Warning);
                mensaje.exec();
            }
        }
        else
        {
            QMessageBox mensaje;
            mensaje.setText("Error no hay conexion con la base de datos");
            mensaje.setIcon(QMessageBox::Warning);
            mensaje.exec();

        }
    }

    ui->guardar->setEnabled(false);
    ui->eliminar->setEnabled(true);
    ui->modificar->setEnabled(true);
    mRepisa->ActualizarMapa((ObjetoMaestro*)new ArticuloTipo());
    this->close();
}

void FormArticuloTipo::on_eliminar_clicked()
{

}

void FormArticuloTipo::on_modificar_clicked()
{
    Estado=MODIFICAR;
    ui->modificar->setEnabled(false);
    ui->guardar->setEnabled(true);
    ui->eliminar->setEnabled(false);
    ui->nombre->setEnabled(true);
    ui->BotonArchivo->setEnabled(true);
}

void FormArticuloTipo::Ruta(QString Cadena)
{
    QString fileName = Cadena;

   if (!fileName.isEmpty())
   {
     QPixmap*  pix=new QPixmap(RutaImagenes+fileName);

       ui->imagenlabel->setPixmap(pix->scaled(60,60,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
       ui->imagen->setText(fileName);

    }
}
