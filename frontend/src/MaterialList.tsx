import { useEffect, useState } from "react";
import { Spinner, Table, Alert, Button, Modal, Form, Col, Row, Container } from "react-bootstrap";
import { DbMaterial } from "./DatabaseInterface";
import CopyButton from "./ui/CopyButton";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import axios from 'axios';
import NewButton from "./ui/NewButton";

type MaterialDialogProps = {
   projectId: string;
   show: boolean;
   onHide: () => void;
}

const MaterialDialog = (props: MaterialDialogProps) => {

   const [data, setData] = useState<DbMaterial>({
      id: 0,
      uuid: "",
      name: "",
      E_modulus: 0
   })

   const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
      const { name, value } = e.target;
      setData({ ...data, [name]: value, });
   };

   const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
      event.preventDefault();
      await axios.post(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId, data)
         .catch((e) => {
            //! \todo show error message
            console.log(e)
         })
      window.location.reload();
   };

   return (
      <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
         <Modal.Header closeButton>
            <Modal.Title id="contained-modal-title-vcenter">Add new material</Modal.Title>
         </Modal.Header>
         <Form onSubmit={handleSubmit}>
            <Modal.Body>
               <Form.Row>
                  <Form.Group as={Col} md="4" controlId="validationCustom01">
                     <Form.Label>Name</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="name"
                        value={data.name}
                        onChange={handleInputChange}
                     />
                     <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                  </Form.Group>
                  <Form.Group as={Col} md="4" controlId="validationCustom02">
                     <Form.Label>E modules [MPa]</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="E_modulus"
                        value={data.E_modulus}
                        onChange={handleInputChange}
                     />
                     {/* <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback> */}
                  </Form.Group>
               </Form.Row>
            </Modal.Body>
            <Modal.Footer>
               <Button type="submit">Submit</Button>
               <Button onClick={props.onHide}>Close</Button>
            </Modal.Footer>
         </Form>
      </Modal>
   );
}

const MaterialListItem = (props: DbMaterial) => {

   const onEdit = () => {
      //! \todo
   }
   const onCopy = () => {
      //! \todo
   }
   const onDelete = async () => {
      const res = await axios.delete(process.env.REACT_APP_BACKEND + '/material/' + props.id)
      if (res.data.success === false) {
         //! \todo handle error
      }
      window.location.reload();
   }
   return (
      <tr>
         <td><EditButton onClick={onEdit} disabled={true}></EditButton></td>
         <td>{props.name}</td>
         <td>{props.E_modulus}</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={false} /></td>
      </tr>
   )
}

export type MaterialListProps = {
   projectId: string;
}

const MaterialList = (props: MaterialListProps) => {

   const [data, setData] = useState<DbMaterial[]>([])
   const [loading, setLoading] = useState(true)
   const [showNewDialog, setShowNewDialog] = useState(false);

   const getData = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId)
         .then(response => setData(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getData() }, []);

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <Container>
         <Row>
            <h4>Materials</h4>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} />
         </Row>
         {data.length === 0 && <Alert variant='danger'>No material defined.</Alert>}
         <Table borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>E modules [MPa]</th>
               </tr>
            </thead>
            <tbody>
               {data.map(e => (
                  <MaterialListItem
                     key={e.id}
                     id={e.id}
                     name={e.name}
                     uuid={e.uuid}
                     E_modulus={e.E_modulus}
                  />))}
            </tbody>
         </Table>

         <MaterialDialog
            projectId={props.projectId}
            show={showNewDialog}
            onHide={() => setShowNewDialog(false)}
         />
      </Container >
   )
}

export default MaterialList;